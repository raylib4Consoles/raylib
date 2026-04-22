#include "raylib.h"         // Declares module functions

// Check if config flags have been externally provided on compilation line
#if !defined(EXTERNAL_CONFIG_FLAGS)
    #include "config.h"     // Defines module configuration flags
#endif

//#include "utils.h"          // Required for: TRACELOG(), LoadFileData(), LoadFileText(), SaveFileText()
#include "rlgl.h"           // OpenGL abstraction layer to OpenGL 1.1, 2.1, 3.3+ or ES2
#include "raymath.h"        // Required for: Vector3, Quaternion and Matrix functionality

#include <stdio.h>          // Required for: sprintf()
#include <stdlib.h>         // Required for: malloc(), free()
#include <string.h>         // Required for: memcmp(), strlen()
#include <math.h>           // Required for: sinf(), cosf(), sqrtf(), fabsf()

#include "rayspine.h"

Texture2D gtexture;
Texture2D tm_textures[MAX_TEXTURES] = {0};
int texture_index = 0;
float anti_z_fighting_index = SP_LAYER_SPACING_BASE;
float worldVerticesPositions[MAX_VERTICES_PER_ATTACHMENT];
VertexSpine vertices[MAX_VERTICES_PER_ATTACHMENT];
int VERTEX_ORDER_NORMAL[] = {0, 1, 2, 4};
int VERTEX_ORDER_INVERSE[] = {4, 2, 1, 0};


void SpineAddVertex(float x, float y, float u, float v, float r, float g, float b, float a, int* index) 
{
	VertexSpine* vertex = &vertices[*index];
	vertex->x = x;
	vertex->y = y;
	vertex->u = u;
	vertex->v = v;
	vertex->r = r;
	vertex->g = g;
	vertex->b = b;
	vertex->a = a;
 	//TraceLog(LOG_INFO,"%s val=%d x=%f y=%f u=%f v=%f r=%f g=%f b=%f a=%f index=%d",__FUNCTION__,x,y,u,v,r,g,b,a,*index);

	*index += 1;
}
Texture2D *SpineCreateTexture2d(char *path) 
{
	TRACELOG(LOG_INFO, "%s %s ",__FUNCTION__,path);

	/*Image spineboy=LoadImage(path);
    ImageFormat(&spineboy, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    tm_textures[texture_index] = LoadTextureFromImage(spineboy);
    UnloadImage(spineboy);
	//tm_textures[texture_index] = LoadTexture(path);
	gtexture=tm_textures[texture_index];
	Texture2D *t = &tm_textures[texture_index];
	//SetTextureFilter(*t,TEXTURE_FILTER_BILINEAR);
	texture_index++;
	rerurn t;*/

	tm_textures[texture_index] = LoadTexture(path);
    gtexture=tm_textures[texture_index];
    Texture2D *t = &tm_textures[texture_index];
    SetTextureFilter(*t, TEXTURE_FILTER_BILINEAR);
    texture_index++;
    return t;
}

void SpineTextureDestroy()
{
	while(texture_index--) UnloadTexture(tm_textures[texture_index]);
}

void SpineDrawRegion(VertexSpine* vertices, Texture* texture, Vector3 position, int* vertex_order)
{
	VertexSpine vertex;
//	TRACELOG(LOG_INFO, "%s %d",__FUNCTION__,texture->id);

	rlSetTexture(texture->id);
	rlPushMatrix();
	{
		rlBegin(RL_QUADS);
		{
			rlColor4f(1.0, 1.0, 1.0, 1.0);
			rlNormal3f(0.0f, 0.0f, 1.0f);
			for (int i = 0; i < 4; i++)
			{
				vertex = vertices[vertex_order[i]];
				rlTexCoord2f(vertex.u, vertex.v);
				rlVertex3f( position.x + vertex.x, position.y + vertex.y, position.z + anti_z_fighting_index);
				//TraceLog(LOG_INFO,"%s  i=%d x=%f y=%f u=%f v=%f position.x=%f position.y=%f position.z=%f anti_z_fighting_index=%f vertex_order=%d",__FUNCTION__,i,vertex.x,vertex.y,vertex.u,vertex.v,position.x,position.y,position.z,anti_z_fighting_index,vertex_order[i]);

			}
		}
		rlEnd();

#ifdef SP_DRAW_DOUBLE_FACED
		rlBegin(RL_QUADS);
		{
			rlNormal3f(0.0f, 0.0f, 1.0f);
			for (int i = 3; i >= 0; i--)
			{
				vertex = vertices[vertex_order[i]];
				rlTexCoord2f(vertex.u, vertex.v);
				rlColor4f(vertex.r, vertex.g, vertex.b, vertex.a);
				rlVertex3f( position.x + vertex.x, position.y + vertex.y, position.z - anti_z_fighting_index);
			}
		}
		rlEnd();
#endif

	}
	rlPopMatrix();
	rlSetTexture(0);
}
void SpineDrawMesh(VertexSpine *vertices, int start, int count, Texture *texture, Vector3 position, int *vertex_order)
{
	VertexSpine vertex;
	//TRACELOG(LOG_INFO, "%s %d",__FUNCTION__,texture->id);

	rlPushMatrix();
	{
		for(int vertexIndex = start; vertexIndex < count; vertexIndex += 3)
		{
			rlEnableTexture(texture->id);
			rlBegin(RL_QUADS);
			{
				int i;
				for (i = 2; i > -1; i--)
				{
					vertex = vertices[vertexIndex + i];
					rlTexCoord2f(vertex.u, vertex.v);
					rlColor4f(vertex.r, vertex.g, vertex.b, vertex.a);
					rlVertex3f(position.x + vertex.x, position.y + vertex.y, position.z + anti_z_fighting_index);
				}
				rlVertex3f(position.x + vertex.x, position.y + vertex.y, position.z + anti_z_fighting_index);
            }
			rlEnd();
#ifdef SP_DRAW_DOUBLE_FACED
			TraceLog(LOG_INFO,"double sided not supported for mesh based spine files\n");
            return;
#endif

#ifdef SP_RENDER_WIREFRAME
			DrawTriangleLines((Vector2) {vertices[vertexIndex].x + position.x, vertices[vertexIndex].y + position.y},
                                  (Vector2) {vertices[vertexIndex + 1].x + position.x, vertices[vertexIndex + 1].y + position.y},
                                  (Vector2) {vertices[vertexIndex + 2].x + position.x, vertices[vertexIndex + 2].y + position.y}, vertexIndex == 0 ? RED : GREEN);
#endif
		}
	}
	rlPopMatrix();
	rlDisableTexture();
}
void SpineDrawSkeleton(spSkeleton *skeleton, Vector3 position)
{

	int *vertex_order = (skeleton->scaleX * skeleton->scaleY < 0) ? VERTEX_ORDER_NORMAL : VERTEX_ORDER_INVERSE;
	anti_z_fighting_index = SP_LAYER_SPACING_BASE;

	// For each slot in the draw order array of the skeleton
	for (int i = 0; i <skeleton->slotsCount; ++i)
	{    
		//TraceLog(LOG_INFO,"%s %d",__FUNCTION__,i);
		anti_z_fighting_index -= SP_LAYER_SPACING;
		spSlot *slot = skeleton->drawOrder[i];

		// Fetch the currently active attachment, continue
		// with the next slot in the draw order if no
		// attachment is active on the slot
		spAttachment *attachment = slot->attachment;
		if(!attachment) continue;

		// Calculate the tinting color based on the skeleton's color
		// and the slot's color. Each color channel is given in the
		// range [0-1], you may have to multiply by 255 and cast to
		// and int if your engine uses integer ranges for color channels.
		float tintR = skeleton->color.r * slot->color.r;
		float tintG = skeleton->color.g * slot->color.g;
		float tintB = skeleton->color.b * slot->color.b;
		float tintA = skeleton->color.a * slot->color.a;

		// Fill the vertices array depending on the type of attachment
		Texture *texture = 0;
		int vertexIndex = 0;
		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			//TraceLog(LOG_INFO,"%s %d",__FUNCTION__,i);

			// Cast to an spRegionAttachment so we can get the rendererObject
			// and compute the world vertices
			spRegionAttachment *regionAttachment = (spRegionAttachment *) attachment;

			// Our engine specific Texture is stored in the spAtlasRegion which was
			// assigned to the attachment on load. It represents the texture atlas
			// page that contains the image the region attachment is mapped to
			texture = (Texture *) ((spAtlasRegion *) regionAttachment->rendererObject)->page->rendererObject;
			//TraceLog(LOG_INFO,"%s texture id=%d",__FUNCTION__,texture->id);

			// Computed the world vertices positions for the 4 vertices that make up
			// the rectangular region attachment. This assumes the world transform of the
			// bone to which the slot (and hence attachment) is attached has been calculated
			// before rendering via spSkeleton_updateWorldTransform
			spRegionAttachment_computeWorldVertices(regionAttachment, slot, worldVerticesPositions, 0, 2);

			// Create 2 triangles, with 3 vertices each from the region's
			// world vertex positions and its UV coordinates (in the range [0-1]).
			SpineAddVertex(worldVerticesPositions[0], worldVerticesPositions[1],
						regionAttachment->uvs[0], regionAttachment->uvs[1],
						tintR, tintG, tintB, tintA, &vertexIndex);

			SpineAddVertex(worldVerticesPositions[2], worldVerticesPositions[3],
						regionAttachment->uvs[2], regionAttachment->uvs[3],
						tintR, tintG, tintB, tintA, &vertexIndex);

			SpineAddVertex(worldVerticesPositions[4], worldVerticesPositions[5],
						regionAttachment->uvs[4], regionAttachment->uvs[5],
						tintR, tintG, tintB, tintA, &vertexIndex);

			SpineAddVertex(worldVerticesPositions[4], worldVerticesPositions[5],
						regionAttachment->uvs[4], regionAttachment->uvs[5],
						tintR, tintG, tintB, tintA, &vertexIndex);

			SpineAddVertex(worldVerticesPositions[6], worldVerticesPositions[7],
						regionAttachment->uvs[6], regionAttachment->uvs[7],
						tintR, tintG, tintB, tintA, &vertexIndex);

			SpineAddVertex(worldVerticesPositions[0], worldVerticesPositions[1],
						regionAttachment->uvs[0], regionAttachment->uvs[1],
						tintR, tintG, tintB, tintA, &vertexIndex);
			//addVertexjander();
			//if(i==7)
			SpineDrawRegion(vertices, texture, position, vertex_order);
		} 
		else if(attachment->type == SP_ATTACHMENT_MESH)
		{
			// Cast to an spMeshAttachment so we can get the rendererObject
			// and compute the world vertices
			spMeshAttachment *mesh = (spMeshAttachment *) attachment;

			// Check the number of vertices in the mesh attachment. If it is bigger
			// than our scratch buffer, we don't render the mesh. We do this here
			// for simplicity, in production you want to reallocate the scratch buffer
			// to fit the mesh.
			if (mesh->super.worldVerticesLength > MAX_VERTICES_PER_ATTACHMENT) continue;

			// Our engine specific Texture is stored in the spAtlasRegion which was
			// assigned to the attachment on load. It represents the texture atlas
			// page that contains the image the mesh attachment is mapped to
			texture = (Texture *) ((spAtlasRegion *) mesh->rendererObject)->page->rendererObject;

			// Computed the world vertices positions for the vertices that make up
			// the mesh attachment. This assumes the world transform of the
			// bone to which the slot (and hence attachment) is attached has been calculated
			// before rendering via spSkeleton_updateWorldTransform
			spVertexAttachment_computeWorldVertices(SUPER(mesh), slot, 0, mesh->super.worldVerticesLength,worldVerticesPositions, 0, 2);

			// Mesh attachments use an array of vertices, and an array of indices to define which
			// 3 vertices make up each triangle. We loop through all triangle indices
			// and simply emit a vertex for each triangle's vertex.
			for(int i = 0; i < mesh->trianglesCount; ++i)
			{
				int index = mesh->triangles[i] << 1;
				SpineAddVertex(worldVerticesPositions[index], worldVerticesPositions[index + 1],
							mesh->uvs[index], mesh->uvs[index + 1],
							tintR, tintG, tintB, tintA, &vertexIndex);
			}

			// Draw the mesh we created for the attachment
			SpineDrawMesh(vertices, 0, vertexIndex, texture, position, vertex_order);
		}
	}
}
