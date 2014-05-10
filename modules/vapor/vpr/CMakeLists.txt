set(HEADERS
	AutoLink.h
	DynLoad/Library.h
	DynLoad/LibraryException.h
	DynLoad/LibraryFinder.h
	DynLoad/LibraryLoader.h
	DynLoad/LoaderError.h
	DynLoad/SymbolLookupException.h
	IO/BlockIO.h
	IO/BufferObjectReader.h
	IO/BufferObjectWriter.h
	IO/EOFException.h
	IO/FileHandle.h
	IO/FileHandle_t.h
	IO/IOException.h
	IO/IOSys.h
	IO/IOSysBase.h
	IO/ObjectReader.h
	IO/ObjectWriter.h
	IO/Port/Port.h
	IO/Port/SerialPort.h
	IO/Port/SerialPort_t.h
	IO/Port/SerialTypes.h
	IO/Selector.h
	IO/Selector_t.h
	IO/SelectorBase.h
	IO/SerializableObject.h
	IO/Socket/AveragingAllocationStrategy.h
	IO/Socket/ConnectionAbortedException.h
	IO/Socket/ConnectionRefusedException.h
	IO/Socket/ConnectionResetException.h
	IO/Socket/InetAddr.h
	IO/Socket/MaximumAllocationStrategy.h
	IO/Socket/McastReq.h
	IO/Socket/NoPushWriter.h
	IO/Socket/NoRouteToHostException.h
	IO/Socket/SimpleAllocationStrategies.h
	IO/Socket/Socket.h
	IO/Socket/Socket_t.h
	IO/Socket/SocketAcceptor.h
	IO/Socket/SocketConnector.h
	IO/Socket/SocketDatagram.h
	IO/Socket/SocketDatagram_t.h
	IO/Socket/SocketException.h
	IO/Socket/SocketOptions.h
	IO/Socket/SocketStream.h
	IO/Socket/SocketStream_t.h
	IO/Socket/SocketTypes.h
	IO/Socket/UnknownHostException.h
	IO/Stats/BandwidthIOStatsStrategy.h
	IO/Stats/BaseIOStatsStrategy.h
	IO/Stats/IOStatsStrategyAdapter.h
	IO/TimeoutException.h
	IO/WouldBlockException.h
	IO/XMLObjectReader.h
	IO/XMLObjectWriter.h
	md/BOOST/IO/IOSysBOOST.h
	md/BOOST/IO/SelectorImplBOOST.h
	md/BOOST/IO/Socket/InetAddrBOOST.h
	md/BOOST/IO/Socket/SocketDatagramImplBOOST.h
	md/BOOST/IO/Socket/SocketImplBOOST.h
	md/BOOST/IO/Socket/SocketStreamImplBOOST.h
	md/BOOST/vprTypesBOOST.h
	md/NSPR/DynLoad/LibraryNSPR.h
	md/NSPR/IO/IOSysNSPR.h
	md/NSPR/IO/SelectorImplNSPR.h
	md/NSPR/IO/Socket/InetAddrNSPR.h
	md/NSPR/IO/Socket/SocketDatagramImplNSPR.h
	md/NSPR/IO/Socket/SocketImplNSPR.h
	md/NSPR/IO/Socket/SocketStreamImplNSPR.h
	md/NSPR/NSPRHelpers.h
	md/NSPR/Sync/CondVarNSPR.h
	md/NSPR/Sync/MutexNSPR.h
	md/NSPR/Sync/RWMutexNSPR.h
	md/NSPR/Sync/SemaphoreNSPR.h
	md/NSPR/SystemNSPR.h
	md/NSPR/Thread/ThreadKeyNSPR.h
	md/NSPR/Thread/ThreadNSPR.h
	md/NSPR/Util/ErrorImplNSPR.h
	md/NSPR/vprTypesNSPR.h
	md/POSIX/DynLoad/LibraryUNIX.h
	md/POSIX/IO/FileHandleImplUNIX.h
	md/POSIX/IO/IOSysUnix.h
	md/POSIX/IO/Port/SerialPortImplTermios.h
	md/POSIX/IO/SelectorImplBSD.h
	md/POSIX/IO/Socket/InetAddrBSD.h
	md/POSIX/IO/Socket/SocketDatagramImplBSD.h
	md/POSIX/IO/Socket/SocketImplBSD.h
	md/POSIX/IO/Socket/SocketStreamImplBSD.h
	md/POSIX/Sync/CondVarPosix.h
	md/POSIX/Sync/MutexPosix.h
	md/POSIX/Sync/RWMutexPosix.h
	md/POSIX/Sync/SemaphorePosix.h
	md/POSIX/SystemPosix.h
	md/POSIX/Thread/ThreadKeyPosix.h
	md/POSIX/Thread/ThreadPosix.h
	md/POSIX/Util/ErrorImplPosix.h
	md/WIN32/DynLoad/LibraryWin32.h
	md/WIN32/IO/Port/SerialPortImplWin32.h
	md/WIN32/Sync/CondVarWin32.h
	md/WIN32/Sync/MutexWin32.h
	md/WIN32/Sync/SemaphoreWin32.h
	md/WIN32/SystemWin32.h
	md/WIN32/Thread/ThreadKeyWin32.h
	md/WIN32/Thread/ThreadWin32.h
	md/WIN32/Util/ErrorImplWin32.h
	Perf/ProfileIterator.h
	Perf/ProfileManager.h
	Perf/ProfileNode.h
	Sync/CondVar.h
	Sync/DeadlockException.h
	Sync/Guard.h
	Sync/GuardedQueue.h
	Sync/LockException.h
	Sync/Mutex.h
	Sync/NullMutex.h
	Sync/RWGuard.h
	Sync/RWMutex.h
	Sync/Semaphore.h
	System.h
	SystemBase.h
	Thread/BaseThread.h
	Thread/Signal.h
	Thread/Thread.h
	Thread/ThreadManager.h
	Thread/ThreadPool.h
	Thread/TSObject.h
	Thread/TSObjectProxy.h
	Thread/TSTable.h
	Thread/UncaughtThreadException.h
	Util/Assert.h
	Util/AttributeMapBase.h
	Util/BadCastException.h
	Util/DateTime.h
	Util/Debug.h
	Util/detail/LifetimeTracker.h
	Util/DurationStatCollector.h
	Util/Error.h
	Util/ErrorBase.h
	Util/Exception.h
	Util/Factory.h
	Util/FileUtils.h
	Util/GUID.h
	Util/IllegalArgumentException.h
	Util/Interval.h
	Util/ResourceException.h
	Util/ReturnStatus.h
	Util/SampleLimitedStatCollector.h
	Util/Singleton.h
	Util/StatCollector.h
	Util/StreamLock.h
	Util/Timer.h
	Util/Version.h
	vpr.h
	vprConfig.h
	vprDomain.h
	vprTypes.h
)

set(SOURCES
	IO/BlockIO.cpp
	IO/BufferObjectReader.cpp
	IO/BufferObjectWriter.cpp
	IO/EOFException.cpp
	IO/IOException.cpp
	IO/ObjectReader.cpp
	IO/ObjectWriter.cpp
	IO/Port/Port.cpp
	IO/Socket/AveragingAllocationStrategy.cpp
	IO/Socket/ConnectionAbortedException.cpp
	IO/Socket/ConnectionRefusedException.cpp
	IO/Socket/ConnectionResetException.cpp
	IO/Socket/MaximumAllocationStrategy.cpp
	IO/Socket/NoPushWriter.cpp
	IO/Socket/NoRouteToHostException.cpp
	IO/Socket/SocketAcceptor.cpp
	IO/Socket/SocketConnector.cpp
	IO/Socket/SocketException.cpp
	IO/Socket/UnknownHostException.cpp
	IO/Stats/BandwidthIOStatsStrategy.cpp
	IO/Stats/BaseIOStatsStrategy.cpp
	IO/TimeoutException.cpp
	IO/WouldBlockException.cpp
	IO/XMLObjectReader.cpp
	IO/XMLObjectWriter.cpp
	md/common/InetAddrHelpers.cpp
	Perf/ProfileIterator.cpp
	Perf/ProfileManager.cpp
	Perf/ProfileNode.cpp
	Sync/DeadlockException.cpp
	Sync/LockException.cpp
	SystemBase.cpp
	Thread/BaseThread.cpp
	Thread/Signal.cpp
	Thread/ThreadManager.cpp
	Thread/ThreadPool.cpp
	Thread/TSObject.cpp
	Thread/TSObjectProxy.cpp
	Thread/UncaughtThreadException.cpp
	Util/BadCastException.cpp
	Util/Debug.cpp
	Util/detail/LifetimeTracker.cpp
	Util/Exception.cpp
	Util/FileUtils.cpp
	Util/GUID.cpp
	Util/IllegalArgumentException.cpp
	Util/Interval.cpp
	Util/ResourceException.cpp
	Util/StreamLock.cpp
	Util/Version.cpp
	vprmain.cpp
)

set(SOURCES_WIN32
	md/WIN32/DynLoad/LibraryWin32.cpp
	md/WIN32/IO/Port/SerialPortImplWin32.cpp
	md/WIN32/Sync/CondVarWin32.cpp
	md/WIN32/Sync/MutexWin32.cpp
	md/WIN32/Sync/SemaphoreWin32.cpp
	md/WIN32/SystemWin32.cpp
	md/WIN32/Thread/ThreadKeyWin32.cpp
	md/WIN32/Thread/ThreadWin32.cpp
	md/WIN32/Util/ErrorImplWin32.cpp
	Sync/RWMutex.cpp
)
source_group(Win32 FILES ${SOURCES_WIN32})

set(SOURCES_NSPR
	md/NSPR/DynLoad/LibraryNSPR.cpp
	md/NSPR/IO/IOSysNSPR.cpp
	md/NSPR/IO/SelectorImplNSPR.cpp
	md/NSPR/IO/Socket/InetAddrNSPR.cpp
	md/NSPR/IO/Socket/SocketDatagramImplNSPR.cpp
	md/NSPR/IO/Socket/SocketImplNSPR.cpp
	md/NSPR/IO/Socket/SocketStreamImplNSPR.cpp
	md/NSPR/NSPRHelpers.cpp
	md/NSPR/Sync/CondVarNSPR.cpp
	md/NSPR/Sync/MutexNSPR.cpp
	md/NSPR/Sync/RWMutexNSPR.cpp
	md/NSPR/SystemNSPR.cpp
	md/NSPR/Thread/ThreadKeyNSPR.cpp
	md/NSPR/Thread/ThreadNSPR.cpp
	md/NSPR/Util/ErrorImplNSPR.cpp
)
source_group(NSPR FILES ${SOURCES_NSPR})

set(SOURCES_BOOST
	md/BOOST/IO/IOSysBOOST.cpp
	md/BOOST/IO/SelectorImplBOOST.cpp
	md/BOOST/IO/Socket/InetAddrBOOST.cpp
	md/BOOST/IO/Socket/SocketDatagramImplBOOST.cpp
	md/BOOST/IO/Socket/SocketImplBOOST.cpp
	md/BOOST/IO/Socket/SocketStreamImplBOOST.cpp
)
source_group(Boost FILES ${SOURCES_BOOST})

set(SOURCES_POSIX
	md/POSIX/DynLoad/LibraryUNIX.cpp
	md/POSIX/IO/FileHandleImplUNIX.cpp
	md/POSIX/IO/IOSysUnix.cpp
	md/POSIX/IO/Port/SerialPortImplTermios.cpp
	md/POSIX/IO/SelectorImplBSD.cpp
	md/POSIX/IO/Socket/InetAddrBSD.cpp
	md/POSIX/IO/Socket/SocketDatagramImplBSD.cpp
	md/POSIX/IO/Socket/SocketImplBSD.cpp
	md/POSIX/IO/Socket/SocketStreamImplBSD.cpp
	md/POSIX/Sync/CondVarPosix.cpp
	md/POSIX/Sync/MutexPosix.cpp
	md/POSIX/Sync/RWMutexPosix.cpp
	md/POSIX/Sync/SemaphorePosix.cpp
	md/POSIX/SystemPosix.cpp
	md/POSIX/Thread/ThreadKeyPosix.cpp
	md/POSIX/Thread/ThreadPosix.cpp
	md/POSIX/Util/ErrorImplPosix.cpp
)
source_group(POSIX FILES ${SOURCES_POSIX})

set(SOURCES_DYNLOAD
	DynLoad/LibraryException.cpp
	DynLoad/LibraryFinder.cpp
	DynLoad/LibraryLoader.cpp
	DynLoad/LoaderError.cpp
	DynLoad/SymbolLookupException.cpp
)
source_group(DynLoad FILES ${SOURCES_DYNLOAD})
list(APPEND SOURCES ${SOURCES_DYNLOAD})

set(SOURCES_MD5RFC
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/libmd5-rfc/md5.c
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/libmd5-rfc/md5.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/libmd5-rfc/md5main.c)
source_group(MD5RFC FILES ${SOURCES_MD5RFC})

set(SOURCES_LEACHUUID
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/leach-uuid/uuid/sysdep.c
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/leach-uuid/uuid/sysdep.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/leach-uuid/uuid/uuid.c
	${CMAKE_CURRENT_SOURCE_DIR}/../../../external/leach-uuid/uuid/uuid.h)
source_group(LeachUUID FILES ${SOURCES_LEACHUUID})

if(WIN32)
	list(APPEND SOURCES ${SOURCES_WIN32})
endif()

if(VPR_USE_BOOST)
	list(APPEND SOURCES ${SOURCES_BOOST})
endif()

if(VPR_USE_NSPR)
	list(APPEND SOURCES ${SOURCES_NSPR})
endif()

if(VPR_USE_POSIX)
	list(APPEND SOURCES ${SOURCES_POSIX})
endif()

if(VPR_USE_LEACH_UUID)
	list(APPEND SOURCES ${SOURCES_LEACHUUID})
endif()

if(VPR_USE_EMBEDDED_MD5RFC)
	list(APPEND SOURCES ${SOURCES_MD5RFC})
endif()


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vprDefines.h.cmake_in ${CMAKE_CURRENT_BINARY_DIR}/vprDefines.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vprParam.h.in ${CMAKE_CURRENT_BINARY_DIR}/vprParam.h)
vrjugglersuite_create_version_rc()

add_definitions(-D_VPR_BUILD_)

add_library(vpr SHARED ${HEADERS} ${SOURCES})
target_link_libraries(vpr
	${UUID_LIBRARY}
	${CPPDOM_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	${DL_LIBRARY})

if(WIN32)
	if(MSVC)
		target_link_libraries(vpr dbghelp)
	else()
		target_link_libraries(vpr imagehlp)
	endif()
	target_link_libraries(vpr rpcrt4 ws2_32)
endif()

set_target_properties(vpr PROPERTIES FOLDER "${VRJUGGLERSUITE_CORELIB_PROJECT_FOLDER}")

vrjugglersuite_set_versioned_library_output_name(vpr)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vprParam.h" "${CMAKE_CURRENT_BINARY_DIR}/vprDefines.h"
	DESTINATION ${VPR_INCLUDE_PREFIX}/vpr
	COMPONENT vpr-dev)

install(TARGETS vpr EXPORT vprexport
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT vpr-runtime
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT vpr-runtime
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT vpr-dev)
