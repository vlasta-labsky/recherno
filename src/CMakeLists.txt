cmake_minimum_required(VERSION 3.12)

set(PROJECT_SOURCES
     main.cpp
     virtualmachine.cpp
     mainwindow.cpp
     impl.cpp
     api.cpp
     systemsummary.cpp
     feedwater.cpp
     loop1.cpp
     loop2.cpp
     condenser.cpp
     deareator.cpp
     reactor.cpp
     emergencycorecooling.cpp
     offlinecooling.cpp
     reactorcontrolrods.cpp
     turbinesupport.cpp
     turbine.cpp

     ../include/api.h
     ../include/impl.h
     ../include/virtualmachine.h

     ../include/condenser.h
     ../include/deareator.h
     ../include/emergencycorecooling.h
     ../include/feedwater.h
     ../include/loop1.h
     ../include/loop2.h
     ../include/mainwindow.h
     ../include/offlinecooling.h
     ../include/reactor.h
     ../include/reactorcontrolrods.h
     ../include/systemsummary.h
     ../include/turbine.h
     ../include/turbinesupport.h

     ui/mainwindow.ui
     ui/systemsummary.ui
     ui/feedwater.ui
     ui/loop1.ui
     ui/loop2.ui
     ui/condenser.ui
     ui/deareator.ui
     ui/reactor.ui
     ui/emergencycorecooling.ui
     ui/offlinecooling.ui
     ui/reactorcontrolrods.ui
     ui/turbinesupport.ui
     ui/turbine.ui
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chernobyl
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
   add_executable(chernobyl
       ${PROJECT_SOURCES}
   )
endif()


target_link_libraries(chernobyl PRIVATE  Qt${QT_VERSION_MAJOR}::Widgets ${UNICORN_LIBRARIES} ${CAPSTONE_LIBRARIES} )

install(TARGETS chernobyl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chernobyl)
endif()
