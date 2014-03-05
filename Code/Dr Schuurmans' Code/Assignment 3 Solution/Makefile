
#Linux:#
#FLAGS = -I/usr/include -I/opt/local/include/eigen3 -L/usr/lib -lglut -lGL -lGLU -lX11 -lm 

#Mac:#
FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/local/include/eigen3

modelViewer: modelViewer.cpp camera.h mesh.*
	g++ modelViewer.cpp mesh.cpp -o modelViewer $(FLAGS)

modelSolidViewer: modelSolidViewer.cpp camera.h light.h mesh.*
	g++ modelSolidViewer.cpp mesh.cpp -o modelSolidViewer $(FLAGS)

motionViewer: motionViewer.cpp camera.h skeleton.* motion.* bvh_util.*
	g++ motionViewer.cpp skeleton.cpp motion.cpp bvh_util.cpp -o motionViewer $(FLAGS)

modelSkeletonViewer: modelSkeletonViewer.cpp camera.h mesh.* skeleton.* motion.* bvh_util.* attachment.*
	g++ modelSkeletonViewer.cpp mesh.cpp skeleton.cpp motion.cpp bvh_util.cpp attachment.cpp -o modelSkeletonViewer $(FLAGS)

personViewer: personViewer.cpp light.h camera.h mesh.* skeleton.* motion.* bvh_util.* attachment.* animation.*
	g++ personViewer.cpp mesh.cpp skeleton.cpp motion.cpp bvh_util.cpp attachment.cpp animation.cpp -o personViewer $(FLAGS)

%: %.cpp
	g++ $*.cpp -o $* $(FLAGS)

clean:
	rm *Viewer
