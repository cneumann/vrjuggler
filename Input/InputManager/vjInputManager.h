/////////////////////////////////////////////////////////////////////////////
// vjInputManager.h
//
// Group manager for vj input devices and proxies
//
////////////////////////////////////////////////////////////////////////////
#ifndef VJ_INPUT_MANAGER_H
#define VJ_INPUT_MANAGER_H

#include <vjConfig.h>
#include <map>

#include <Kernel/vjDebug.h>
#include <Kernel/vjConfigChunkHandler.h>

// Proxies
#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjKeyboardProxy.h>
#include <Input/InputManager/vjGestureProxy.h>

// Dummies
#include <Input/vjPosition/vjDummyPosition.h>
#include <Input/vjInput/vjDummyDigital.h>
#include <Input/vjInput/vjDummyAnalog.h>
#include <Input/vjGlove/vjDummyGlove.h>
#include <Input/vjKeyboard/vjDummyKeyboard.h>
#include <Input/vjGesture/vjDummyGesture.h>

//: The InputManager holds an manages all vj Input devices.
//
//  The InputManager, handles all the
//  details behind organizing the input devices in the library.  It
//  provides an API for adding devices by their configChunk and deleting
//  them by their chunk (or just their string name).
//
//  Proxies are used to abstract away from the devices so any type and number
//  of devices may be accessed in the same way.  These proxies are also
//  set up by configChunks and should be accessed by number, rather than name.
//  (for speed)
//-------------------------------------------------------------------------------
//!PUBLIC_API:
class vjInputManager : public vjMemory, public vjConfigChunkHandler
{
public:
   vjInputManager();
   ~vjInputManager();

   friend ostream& operator<<(ostream& out, vjInputManager& iMgr);

 //---------------------------//
 //      CONFIG               //
 //---------------------------//
   //: Initial configuration for InputManager
   //! PRE: MUST be called before any config is added to input manager
   void configureInitial(vjConfigChunkDB* cdb);

   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //! POST: (chunk is proxy) ==> (returns == false)
   //+       (chunk is device) ==> (devices is removed && proxies are stupified)
   //+       (chunk is proxyAlias) ==> (proxyAlias is removed && devInterfaces.refreshAll())
   //!RETURNS: success
   bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   bool configCanHandle(vjConfigChunk* chunk);

private:
   //: Load the device for the given chunk
   //!RETURNS: true - Device was configured and added
   bool configureDevice(vjConfigChunk* chunk);

   //: Load the Proxy for the given chunk
   //!RETURNS: true - Proxy was configured and added
   bool configureProxy(vjConfigChunk* chunk);

   //: Remove the device associated with the given chunk
   //!RETURNS: true - Device was removed
   bool removeDevice(vjConfigChunk* chunk);


   //-------------------//
   //     PROXIES       //
   //-------------------//
public:
   //: Function to get an index to the proxy/alias given in str
   //! RETURNS: -1 - Not Found
   int  getProxyIndex(std::string proxyName);


   /* ------------------------------- //
   //        DEVICE API               //
   // ------------------------------- */
public:
   //: Update the Data in all the devices.
   //
   // Call UpdateData() on all the devices in the devArray
   // Also updates all proxies.
   //
   //! MODIFIES: instances pointed to by devArray
   void updateAllData();

private:
   //: Add a device to vjInputManager.
   //
   // Add the devPtr to the device Array, devPtr should
   // not already be in the array.  Returns -1 on failure
   //
   //! MODIFIES: self
   //! POST: m_devVector' = m_devVector \/ devPtr
   //+       return = devNum (position in the array)
   //                or -1 for fail
   int fAddDevice(vjInput* devPtr);

   //: Remove a device from the vjInputManager.
   // Remove the device at position devNum from the
   // device Array.  Returns true on success.
   //
   //! MODIFIES: self
   //! POST: m_devVector[devNum]' = NULL
   bool removeDevice(int devNum);
   bool removeDevice(char* instName);

   //: Get the device number from its Instance Name.
   //
   //  Find the devNum of the device whose name is instName
   //
   //! RETURNS: -1 if fails
   int findDeviceNum(const char* instName);

   //: Get a pointer to the instance at devNum.
   // return the pointer at the devNum position in the array. <br>
   //
   //!POST: return = m_devVector[devNum]  (this can be NULL)
   vjInput* getDevice(int devNum);

   /*********************************************************
    *          PROXIES                                      *
    *********************************************************/
public:
   /**********     POS PROXY   ****************************/

   //: Set the index ProxyNum in the posProxy array to proxy
   // device at devNum in the device array, with subNumber <br>
   //
   //! MODIFIES: self <br>
   //! POST: m_posProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int setPosProxy(int ProxyNum, int DevNum, int subNum);

   //: Get the proxy at the given index
   vjPosProxy* getPosProxy(int posProxyIndex)
   {
      vjASSERT(m_posProxyVector.size() > posProxyIndex);    // Check array bounds
      return m_posProxyVector[posProxyIndex];
   }

   //: Add the pos proxy
   //! POST: pos proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addPosProxy(std::string devName, int subNum, std::string proxyName, vjPosProxy* posProxy);

   //: Turn the position proxy at index ProxyNum to point back
   // to the default dummy proxy. <br>
   // <br>
   //! MODIFIES: self <br>
   //! POST:  m_posProxyArray[proxyNum]' = m_dummyPos
   void stupifyPos(int ProxyNum);


/***********       DIGITAL PROXY   * ******************/
   //: Set the index ProxyNum in the digProxy array to proxy
   // device at devNum in the device array, with subNumber <br>
   // <br>
   // modifies: self <br>
   // post: m_digProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int setDigProxy(int ProxyNum, int DevNum, int subNum);

   //: Get the proxy at the given index
   vjDigitalProxy* getDigProxy(int digProxyIndex)
   {
      vjASSERT(m_digProxyVector.size() > digProxyIndex);    // Check array bounds
      return m_digProxyVector[digProxyIndex];
   }

   //: Add the digital proxy
   //! POST: dig proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addDigProxy(std::string devName, int subNum, std::string proxyName, vjDigitalProxy* digitalProxy);

   //: Turn the digital proxy at index ProxyNum to point back
   // to the default dummy proxy.<br>
   // <br>
   //! MODIFIES: self<br>
   //! POST:  m_digProxyArray[proxyNum]' = m_dummyDig
   void stupifyDig(int ProxyNum);


/*************** ANALOG PROXY *******************************/
   //: Set the index ProxyNum in the anaProxy array to proxy
   // device at devNum in the device array, with subNumber<br>
   // <br>
   // modifies: self<br>
   // post: m_anaProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int setAnaProxy(int ProxyNum, int DevNum, int subNum);

   //: Get the proxy at the given index
   vjAnalogProxy* getAnaProxy(int anaProxyIndex)
   {
      vjASSERT(m_anaProxyVector.size() > anaProxyIndex);    // Check array bounds
      return m_anaProxyVector[anaProxyIndex];
   }

   //: Add the analog proxy
   //! POST: analog proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addAnaProxy(std::string devName, int subNum, std::string proxyName, vjAnalogProxy* anaProxy);

   //: Turn the analog proxy at index ProxyNum to point back
   // to the default dummy proxy.<br>
   //<br>
   //!MODIFIES: self<br>
   //!POST:  m_anaProxyArray[proxyNum]' = m_dummyAna
   void stupifyAna(int ProxyNum);


/******************     GLOVE PROXY   ************************/
   //: Set the index ProxyNum in the gloveProxy array to proxy
   // device at devNum in the device array, with subNumber <br>
   // <br>
   //!MODIFIES: self <br>
   //!POST: m_gloveProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int setGloveProxy(int ProxyNum, int DevNum, int subNum);

   //: get the proxy at the given index
   vjGloveProxy* GetGloveProxy(int gloveProxyIndex)
   {
      vjASSERT(m_gloveProxyVector.size() > gloveProxyIndex);    // Check array bounds
      return m_gloveProxyVector[gloveProxyIndex];
   }

   //: Add the glove proxy
   //! POST: glove proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addGloveProxy(std::string devName, int subNum, std::string proxyName, vjGloveProxy* gloveProxy);

   //: Turn the glove proxy at index ProxyNum to point back
   // to the default dummy glove proxy. <br>
   // <br>
   //!MODIFIES: self <br>
   //!POST:  m_gloveProxyArray[proxyNum]' = m_dummyGlove
   void stupifyGlove(int ProxyNum);

   //: Find out how many glove proxies are in the system currently
   // XXX: Experimental, needed to draw the glove objects
   int getNumGloveProxies()
   { return m_gloveProxyVector.size(); }

/*****************       KEYBOARD PROXY        **************/
   //: Set the index ProxyNum in the keyboardProxy array to proxy
   // device at devNum in the device array<br>
   // <br>
   //! MODIFIES: self <br>
   //! POST: m_keyboardProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int setKeyboardProxy(int ProxyNum, int DevNum);

   //: Get the keyboard proxy at the given index
   vjKeyboardProxy* getKeyboardProxy(int keyboardProxyIndex)
   {
      vjASSERT(m_keyboardProxyVector.size() > keyboardProxyIndex);    // Check array bounds
      return m_keyboardProxyVector[keyboardProxyIndex];
   }

   //: Add the keyboard proxy
   //! POST: keyboard proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addKeyboardProxy(std::string devName, int subNum, std::string proxyName, vjKeyboardProxy* kbProxy);

   //: Turn the keyboard proxy at index ProxyNum to point back
   // to the default dummy keyboard proxy. <br>
   // <br>
   //! MODIFIES: self <br>
   //! POST:  m_keyboardProxyArray[proxyNum]' = m_dummyKeyboard
   void stupifyKeyboard(int ProxyNum);

/*******************       GESTURE PROXY    ***************************/
   //: Set the index ProxyNum in the gestureProxy array to proxy
   // device at devNum in the device array<br>
   // <br>
   //! MODIFIES: self <br>
   //! POST: m_gestureProxyArray[proxyNum]' = new Proxy at DevNum
   //       return = 0 for fail, other for success
   int setGestureProxy(int ProxyNum, int DevNum);

   //: Get the gesture proxy at the given index
   vjGestureProxy* getGestureProxy(int gestureProxyIndex)
   {
      vjASSERT(m_gestureProxyVector.size() > gestureProxyIndex);    // Check array bounds
      return m_gestureProxyVector[gestureProxyIndex];
   }

   //: Add the gesture proxy
   //! POST: gesture proxy has been added
   //+   proxy alias has been set
   //! RETURNS: -1: failure, >0: proxy_num
   int addGestureProxy(std::string devName, int subNum, std::string proxyName, vjGestureProxy* gestureProxy);

   //: Turn the gesture proxy at index ProxyNum to point back
   // to the default dummy gesture proxy. <br>
   // <br>
   //! MODIFIES: self <br>
   //! POST:  m_gestureProxyArray[proxyNum]' = m_gestureKeyboard
   void stupifyGesture(int ProxyNum);


protected:
      // --- Vectors of devices and proxies --- //
   std::vector<vjInput*>           m_devVector;
   std::vector<vjPosProxy*>        m_posProxyVector;
   std::vector<vjDigitalProxy*>    m_digProxyVector;
   std::vector<vjAnalogProxy*>     m_anaProxyVector;
   std::vector<vjGloveProxy*>      m_gloveProxyVector;
   std::vector<vjKeyboardProxy*>   m_keyboardProxyVector;
   std::vector<vjGestureProxy*>    m_gestureProxyVector;

   vjDummyPosition   m_dummyPos;
   vjDummyDigital    m_dummyDig;
   vjDummyAnalog     m_dummyAna;
   vjDummyGlove      m_dummyGlove;
   vjDummyKeyboard   m_dummyKeyboard;
   vjDummyGesture    m_dummyGesture;

   vjPosProxy*      m_dummyPosProxy;      // Proxy to the dummy pos

   std::map<std::string, int> proxyAliases; // List of alias indices for proxies

private:
   //: Function to configure the proxy Alias array
   bool configureProxyAlias(vjConfigChunk* chunk);

   //: Remove a proxy alias
   bool removeProxyAlias(vjConfigChunk* chunk);

   //: Add a proxy alias
   void addProxyAlias(std::string str, int proxyIndex);
};

// Write out the status of the input manager
ostream& operator<<(ostream& out, vjInputManager& iMgr);

#endif
