# -----------------------------------------------------------------------------
#  K-Engine MAKEFILE
# -----------------------------------------------------------------------------

all:
	CL /c /EHsc ../core/src/kewindow.cpp ../core/src/kewinapiwrapper.cpp ../core/src/kecore.cpp ../core/src/ketimehandler.cpp ../core/src/ketimer.cpp ../core/src/kerenderingsystem.cpp ../core/src/keimage.cpp ../core/src/kemodel.cpp ../core/src/kelight.cpp /I../core/inc
	lib kewindow.obj kewinapiwrapper.obj kecore.obj ketimehandler.obj ketimer.obj kerenderingsystem.obj keimage.obj kemodel.obj kelight.obj /OUT:kengine.lib
	CL ../game/main.cpp /I../core/inc /EHsc /link kengine.lib gdi32.lib opengl32.lib glu32.lib user32.lib /OUT:game.exe
