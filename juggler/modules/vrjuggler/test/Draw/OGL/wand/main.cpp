//----------------------------------------
// Test OpenGL VR Juggler program
//
// main.cpp
//----------------------------------------
#include <wandApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>

#ifndef WIN32
#include <sched.h>
#endif

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();           // Get the kernel
   wandApp* application = new wandApp(kernel);       // Instantiate an instance of the app

   // Load any config files specified on the command line
   for(int i=1;i<argc;i++)
      kernel->loadConfigFile(argv[i]);

   kernel->start();

   /*
   for(int i=0;i<20;i++)
   {
      usleep(50000);
      cout << i << "." << flush;
   }
   */

   kernel->setApplication(application);

   while(1)
   {
      usleep(250000);
   }
}
