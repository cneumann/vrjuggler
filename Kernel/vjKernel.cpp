#include <vjConfig.h>
#include <string.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkFactory.h>

// Initailize Statics
vjKernel* vjKernel::_instance = NULL;

/// Start the Kernel running
int vjKernel::start()
{
   if (app == NULL)
   {
      cerr << "vj ERROR: Can't start Kernel until setting App" << endl;
      return -1;
   }

   // Create a new thread to handle the control
   vjThreadMemberFunctor<vjKernel>* memberFunctor =
   new vjThreadMemberFunctor<vjKernel>(this, &vjKernel::controlLoop, NULL);

   mControlThread = new vjThread(memberFunctor, 0);

   vjDEBUG(0) << "vjKernel::start: Just started control loop.  "
              << mControlThread << endl << vjDEBUG_FLUSH;

   return 1;
}

//-----------------------------------------------
// Initialize Shared Memory
// Load config
// Setup Input, Display, and then Draw Managers
//!NOTE: Does initial configuration and then sends config file to configAdd
//!POST: Shared Memory Initialized
//----------------------------------------------
void vjKernel::initConfig()
{
      vjDEBUG_BEGIN(0) << "vjKernel::initConfig: Entered.\n" << vjDEBUG_FLUSH;

   vjSharedPool::init();         // Try to init the pool stuff
   sharedMemPool = new vjSharedPool(1024*1024);      // XXX: should not be system specific

   // --- Make sure chunks have been initialized --- //
   //loadConfigFile(std::string(""));

   // Setup initial environments.
   initialSetupInputManager();
   initialSetupDisplayManager();
   setupEnvironmentManager();

   configAdd(mChunkDB);       // Setup the configuration

   //apiFactory = app->api.getAPIFactory();
   sysFactory = vjSGISystemFactory::instance(); // XXX: Should not be system specific

      vjDEBUG(0) << "vjKernel::initConfig: Calling setupDrawManager.\n" << vjDEBUG_FLUSH;
   setupDrawManager();
   displayManager->setDrawManager(drawManager);
      vjDEBUG_END(0) << "vjKernel::initConfig: Exiting.\n" << vjDEBUG_FLUSH;
}

/// The Kernel loop
void vjKernel::controlLoop(void* nullParam)
{
   vjDEBUG(0) << "vjKernel::controlLoop: Entered.\n" << vjDEBUG_FLUSH;

   initConfig();

   // setup performance buffer
   performanceEnabled = 0;
   std::vector<vjConfigChunk*>* perfchunks = mChunkDB->getMatching ("PerfMeasure");
   if (perfchunks->size() > 0) {
       vjConfigChunk* perfchunk = (*perfchunks)[0];
       performanceEnabled = (*perfchunks)[0]->getProperty ("KernelEnabled");
       delete perfchunks;
   }
   perfBuffer = new vjPerfDataBuffer ("Kernel loop",
				      500, 7, performanceEnabled);
   environmentManager->addPerfDataBuffer (perfBuffer);


   //while(!Exit)
   while (1)
   {
       // perfBuffer->set (0);
         vjDEBUG(3) << "vjKernel::controlLoop: app->preDraw()\n" << vjDEBUG_FLUSH;
      app->preDraw();         // Do Any application pre-draw stuff
      perfBuffer->set (0);
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->draw()\n" << vjDEBUG_FLUSH;
      drawManager->draw();    // Trigger the beginning of frame drawing
      perfBuffer->set (1);
         vjDEBUG(3) << "vjKernel::controlLoop: app->postDraw\n" << vjDEBUG_FLUSH;
      app->postDraw();        // Do computations that can be done while drawing.  This should be for next frame.
      perfBuffer->set (2);
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->sync()\n" << vjDEBUG_FLUSH;
      drawManager->sync();    // Block until drawing is done
      perfBuffer->set (3);
         vjDEBUG(3) << "vjKernel::controlLoop: app->postSync()\n" << vjDEBUG_FLUSH;
      app->postSync();        // Do processing after drawing is complete
      perfBuffer->set (4);
      // Sync should be here for Kernel changes from
      // the environment manager

      //Tell trackers to swap buffers;
         vjDEBUG(3) << "vjKernel::controlLoop: Update Trackers\n" << vjDEBUG_FLUSH;
      getInputManager()->UpdateAllData();
      perfBuffer->set(5);
         vjDEBUG(3) << "vjKernel::controlLoop: Update Projections\n" << vjDEBUG_FLUSH;
      drawManager->updateProjections();
      perfBuffer->set(6);
   }
}

void vjKernel::updateFrameData()
{
   drawManager->updateProjections();
}

//---------------------------------
// Config routines
//---------------------------------
void vjKernel::configAdd(vjConfigChunkDB* chunkDB)
{
   // Dependency sort the items
   int dep_result = chunkDB->dependencySort();

   // If sort fails, exit with error
   if(dep_result == -1)
   {
      vjDEBUG(0) << "vjKernel::configAdd: ERROR: Dependency sort failed. Aborting add.\n" << vjDEBUG_FLUSH;
      return;
   }

   // Get sorted list
   std::vector<vjConfigChunk*> chunks = chunkDB->getChunks();

   // For each element
   for(int i=0;i<chunks.size();i++)
   {
      bool added_chunk = false;

      vjDEBUG(1) << "vjKernel::configAdd: chunk: " << (char*)chunks[i]->getProperty("name") << endl << vjDEBUG_FLUSH;
      if(this->configKernelHandle(chunks[i]))            // Kernel
         added_chunk = this->configKernelAdd(chunks[i]);
      if(getInputManager()->configCanHandle(chunks[i]))  // inputMgr
         added_chunk = getInputManager()->configAdd(chunks[i]);
      if(displayManager->configCanHandle(chunks[i]))     // displayMgr
         added_chunk = displayManager->configAdd(chunks[i]);
      // drawMgr
      // app

      // --- Check for adding to active config --- //
      if(added_chunk)      // if added: add to config database
      {}
      else                 // Else: Give unrecognized error
      {
         vjDEBUG(0) << "vjKernel::configAdd: Unrecognized chunk.\n"
                    << "   type: " << (char*)chunks[i]->getType() << endl << vjDEBUG_FLUSH;
      }
   }

   // Dump status
   getInputManager()->DumpStatus();

}

void vjKernel::configRemove(vjConfigChunkDB* chunkDB)
{
   ;
}


// -------------------------------
// Config chunks local to kernel
// -------------------------------
bool vjKernel::configKernelHandle(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)(char*)chunk->getType();

   if(std::string("JugglerUser") == chunk_type)
      return true;
   else
      return false;
}

bool vjKernel::configKernelAdd(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)(char*)chunk->getType();

   vjASSERT(configKernelHandle(chunk));

   if(std::string("JugglerUser") == chunk_type)
   {
      vjUser* new_user = new vjUser;
      bool success = new_user->config(chunk);
      if(!success)
         delete new_user;
      else
         mUsers.push_back(new_user);

      return success;
   }
   else
      return false;
}

bool vjKernel::configKernelRemove(vjConfigChunk* chunk)
{
   return false;
}


// --- STARTUP ROUTINES --- //
void vjKernel::loadConfigFile(std::string filename)
{
   vjDEBUG(1) << "   vjKernel::loadConfigFile: " << filename << endl << vjDEBUG_FLUSH;

   // ------ OPEN chunksDesc file ----- //
   char* vj_base_dir = getenv("VJ_BASE_DIR");
   if(vj_base_dir == NULL)
   {
      vjDEBUG(0) << "vjKernel::loadConfig: Env var VJ_BASE_DIR not defined.\n" << vjDEBUG_FLUSH;
      exit(1);
   }

   if (NULL == mConfigDesc)
   {
      char chunk_desc_file[250];
      strcpy(chunk_desc_file, vj_base_dir);
      strcat(chunk_desc_file, "/Data/chunksDesc");

      mConfigDesc = new vjChunkDescDB;
      vjChunkFactory::setChunkDescDB (mConfigDesc);
      if (!mConfigDesc->load(chunk_desc_file))
      {
         vjDEBUG(0) << "ERROR: vjKernel::loadConfig: Config Desc failed to load file: " << endl << vjDEBUG_FLUSH;
         exit(1);
      }
   }

   if(NULL == mChunkDB)
      mChunkDB = new vjConfigChunkDB(mConfigDesc);      // Create config database


   // ------- OPEN Program specified Config file ------ //
   if(!filename.empty())   // We have a filename
   {
      if (!mChunkDB->load(filename.c_str()))
      {
         vjDEBUG(0) << "ERROR: vjKernel::loadConfig: DB Load failed to load file: " << filename << endl << vjDEBUG_FLUSH;
         exit(1);
      }
   }

   vjDEBUG(5) << "------------  Config Chunks ----------" << vjDEBUG_FLUSH;
   vjDEBUG(5) << (*mChunkDB) << vjDEBUG_FLUSH;
}


void vjKernel::initialSetupInputManager()
{
   vjDEBUG(0) << "   vjKernel::initialSetupInputManager\n" << vjDEBUG_FLUSH;
   mInputManager = new (sharedMemPool) vjInputManager;
   mInputManager->ConfigureInitial(mChunkDB);
}


void vjKernel::initialSetupDisplayManager()
{
   vjDEBUG_BEGIN(0) << "------- vjKernel::initialSetupDisplayManager\n -------" << vjDEBUG_FLUSH;

   // Get display manager
   displayManager = vjDisplayManager::instance();
}

void vjKernel::setupDrawManager()
{
   vjDEBUG_BEGIN(0) << "   vjKernel::setupDrawManager" << endl << vjDEBUG_FLUSH;

   //drawManager = apiFactory->getDrawManager();
   drawManager = app->getDrawManager();
   drawManager->config(mChunkDB);     // Give it the chunk DB to extract API specific info
   drawManager->setApp(app);
   drawManager->setDisplayManager(displayManager);

   app->init();                     // Init the app
   drawManager->initAPI();          // Just sets up API type stuff
   app->apiInit();                  // Have app do any app-init stuff
   drawManager->initDrawing();      // Configs and Starts drawing procs

   vjDEBUG_END(0) << "   vjKernel::setupDrawManager: Exiting." << endl << vjDEBUG_FLUSH;
}


void vjKernel::setupEnvironmentManager() {
   vjDEBUG(0) << "   vjKernel::setupEnvironmentManager\n"
              << vjDEBUG_FLUSH;
   vjTimeStamp::initialize();
   environmentManager = new vjEnvironmentManager();
   environmentManager->activate();
   vjDEBUG(0) << "      Environment Manager running\n" << vjDEBUG_FLUSH;
}

vjUser* vjKernel::getUser(std::string userName)
{
   for(int i=0;i<mUsers.size();i++)
      if(userName == mUsers[i]->getName())
         return mUsers[i];

   return NULL;
}


