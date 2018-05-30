#include <boost/python.hpp>

//RenderingAPIs/OpenGL
extern void init_SPK_GLRenderer_h();
extern void init_SPK_GLExtHandler_h();
extern void init_SPK_GLLineRenderer_h();
extern void init_SPK_GLLineTrailRenderer_h();
extern void init_SPK_GLPointRenderer_h();
extern void init_SPK_GLQuadRenderer_h();

BOOST_PYTHON_MODULE(_GL)
{
	init_SPK_GLRenderer_h();
	init_SPK_GLExtHandler_h();
	init_SPK_GLLineRenderer_h();
	init_SPK_GLLineTrailRenderer_h();
	init_SPK_GLPointRenderer_h();
	init_SPK_GLQuadRenderer_h();
}

int main()
{
	return 0;

}

