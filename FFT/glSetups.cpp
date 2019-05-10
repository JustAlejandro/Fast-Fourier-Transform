#include "glSetups.h"

void frameBufferSetup(GLuint & FrameBuffer, GLuint* mainRenderTex, GLuint& depth, GLenum* DrawBuffers, int width, int height, int buffers)
{
	//FBO setup
	CHECK_GL_ERROR(glGenFramebuffers(1, &FrameBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	CHECK_GL_ERROR(glGenTextures(buffers, mainRenderTex));
	//Setup tex to render to (Supporting multiple channels)
	for (int i = 0; i < buffers; i++) {
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, mainRenderTex[i]));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	//Depth Tex
	CHECK_GL_ERROR(glGenTextures(1, &depth));
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, mainRenderTex[0]));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	for (int i = 0; i < buffers; i++) {
		switch (i) {
		case 0:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mainRenderTex[0], 0);
			break;
		case 1:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mainRenderTex[1], 0);
			break;
		case 2:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, mainRenderTex[2], 0);
			break;
		case 3:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, mainRenderTex[3], 0);
			break;
		case 4:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, mainRenderTex[4], 0);
			break;
		case 5:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, mainRenderTex[5], 0);
			break;
		default:
			std::cout << "TOO MANY COLOR CHANELLS, PROGRAM MORE" << std::endl;
			break;
		}
	}

	//Draw Buffer bind
	CHECK_GL_ERROR(glDrawBuffers(buffers, DrawBuffers));
}

void glCleanup()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void toTextureSetup(int width, int height, GLuint& FrameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	glViewport(0, 0, width, height);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	// Set ranges of the depth buffer to be linear
	glDepthRange(0.0, 1.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
