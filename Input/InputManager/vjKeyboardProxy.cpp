
#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjKeyboardProxy.h>

bool vjKeyboardProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "------------------ KEYBOARD PROXY -----------------\n"
                    << "   config()" << endl << vjDEBUG_FLUSH;
   vjASSERT(((string)(char*)chunk->getType()) == "KeyboardProxy");

   int unitNum = chunk->getProperty("unit");
   string proxy_name = (char*)chunk->getProperty("name");
   string dev_name = (char*)chunk->getProperty("device");

   int proxy_num = vjKernel::instance()->getInputManager()->AddKeyboardProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(1) << "   KeyboardProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(1) << "   KeyboardProxy config() failed" << endl << vjDEBUG_FLUSH;
      return false;
   }
}
