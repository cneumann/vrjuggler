#ifndef _SELECTOR_TEST_H
#define _SELECTOR_TEST_H

#include <vector>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


using namespace vpr;

namespace vprTest
{

class SelectorTest : public CppUnit::ThreadTestCase
{
public:
   SelectorTest()
   : CppUnit::ThreadTestCase ()
   {;}

   SelectorTest(std::string name)
   : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~SelectorTest()
   {}

   virtual void setUp()
   {
      //threadAssertReset();
   }

   virtual void tearDown()
   {
      //checkThreadAssertions();
   }

   // Test an acceptor pool
   // This test is based upon the idea of having a pool of acceptors that
   // Another process keeps connecting too.  A selector is used to determine
   // which acceptor is actually being connected to so that only a single
   // accept is needed (that never should really block)
   void testAcceptorPoolSelection();
   void testAcceptorPoolSelection_acceptor( void* arg );
   void testAcceptorPoolSelection_connector( void* arg );

   // Test sending data to a group of recievers that are pooled
   // This test is based upon the idea of having a pool of
   // connected sockets.  The connector side is sending, and the acceptor side is recieving.
   //
   // The connector side selects some subgroup of the sockets, marks them as selected, and then sends data on them
   // The acceptor side then selects on the sockets and hopefully it gets that same group back as the ones with pending data
   //
   void testSendThenPoll();
   void testSendThenPoll_acceptor(void* arg);
   void testSendThenPoll_connector(void* arg);

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SelectorTest");

#ifndef VPR_SIMULATOR
      test_suite->addTest( new CppUnit::TestCaller<SelectorTest>("testAcceptorPoolSelection", &SelectorTest::testAcceptorPoolSelection));
      test_suite->addTest( new CppUnit::TestCaller<SelectorTest>("testSendThenPoll", &SelectorTest::testSendThenPoll));
#endif

      return test_suite;
   }

protected:
    unsigned     mNumIters;

    vpr::Uint16     mRendevousPort;       // The port the acceptor will be waiting on
    vpr::Uint16     mNumRendevousPorts;   // The number of ports to use

    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    enum State { ACCEPTOR_READY, NOT_READY, CONNECTOR_CLOSED, DATA_SENT, DONE_READING };
    State           mState;         // State variable
    vpr::CondVar    mCondVar;       // Condition variable

    std::vector<vpr::Uint16>     mSelectedPorts;      // List of ports that were selected to send data

};

}       // namespace

#endif
