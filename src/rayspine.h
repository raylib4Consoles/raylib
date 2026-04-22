#include <spine/spine.h>
#include <spine/extension.h>
#ifndef SP_LAYER_SPACING
#define SP_LAYER_SPACING 0
#endif

#ifndef SP_LAYER_SPACING_BASE
#define SP_LAYER_SPACING_BASE 0
#endif
typedef struct VertexSpine 
{
	// Position in x/y plane
	float x, y;

	// UV coordinates
	float u, v;

	// Color, each channel in the range from 0-1
	// (Should really be a 32-bit RGBA packed color)
	float r, g, b, a;
} VertexSpine;
#define MAX_TEXTURES 10
#define MAX_VERTICES_PER_ATTACHMENT 2048


void SpineDrawRegion(VertexSpine* vertices, Texture* texture, Vector3 position, int* vertex_order);
void SpineDrawMesh(VertexSpine *vertices, int start, int count, Texture *texture, Vector3 position, int *vertex_order);
void SpineDrawSkeleton(spSkeleton *skeleton, Vector3 position);
Texture2D *SpineCreateTexture2d(char *path);
void SpineTextureDestroy();
