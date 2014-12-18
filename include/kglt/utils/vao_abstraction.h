#ifndef VAO_ABSTRACTION_H
#define VAO_ABSTRACTION_H

#include "glcompat.h"


#ifdef __ANDROID__
typedef PFNGLGENVERTEXARRAYSOESPROC PFNGLGENVERTEXARRAYSPROC;
typedef PFNGLBINDVERTEXARRAYOESPROC PFNGLBINDVERTEXARRAYPROC;
typedef PFNGLDELETEVERTEXARRAYSOESPROC PFNGLDELETEVERTEXARRAYSPROC;
typedef PFNGLISVERTEXARRAYOESPROC PFNGLISVERTEXARRAYPROC;
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void (GL_APIENTRYP GETINTEGERV) (GLenum pname, GLint * data);
#else
typedef void (*GETINTEGERV) (GLenum pname, GLint * data);
#endif

void vao_init();

extern PFNGLGENVERTEXARRAYSPROC vaoGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC vaoBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC vaoDeleteVertexArrays;
extern PFNGLISVERTEXARRAYPROC vaoIsVertexArray;
extern PFNGLVERTEXATTRIBPOINTERPROC vaoVertexAttribPointer;
extern GETINTEGERV vaoGetIntegerv;

#endif // VAO_ABSTRACTION_H
