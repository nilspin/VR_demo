#define STB_IMAGE_IMPLEMENTATION


#include "Application.h"
#include "stb_image.h"
#include<fstream>

SDL_Event event;

Application::Application() {
  rectangle.resize(bufferWidth*bufferHeight);
  left = stbi_load("assets/left.jpg", &imgWidth, &imgHeight, &channels, 2);
  right = stbi_load("assets/right.jpg", &imgWidth, &imgHeight, &channels, 2);
  if(left == nullptr) {cout<<"could not read first image file!"<<endl; exit(0);}
  if(right == nullptr) {cout<<"could not read second image file!"<<endl; exit(0);}
  //cam.setPosition(glm::vec3(320, 240, 300));
  cam.setPosition(glm::vec3(0,0,100));
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  setupShaders();
  setupBuffers();
  //setupTextures();
}

void Application::run() {
  while (!quit)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
        case SDL_QUIT:	//if X windowkey is pressed then quit
          quit = true;

        case SDL_KEYDOWN:	//if ESC is pressed then quit

          switch (event.key.keysym.sym)
          {
            case SDLK_q:
              quit = true;
              break;

            case SDLK_w:
              cam.move(FORWARD);
              break;

            case SDLK_s:
              cam.move(BACK);
              break;

            case SDLK_a:
              cam.move(LEFT);
              break;

            case SDLK_d:
              cam.move(RIGHT);
              break;

            case SDLK_UP:
              cam.move(UP);
              break;

            case SDLK_DOWN:
              cam.move(DOWN);
              break;

            case SDLK_LEFT:
              cam.move(ROT_LEFT);
              break;

            case SDLK_RIGHT:
              cam.move(ROT_RIGHT);
              break;


            case SDLK_r:
              cam.reset();
              std::cout << "Reset button pressed \n";
              break;
          }
          break;

        case SDL_MOUSEMOTION:
          cam.rotate();
          break;
      }
    }

    //First things first
    cam.calcMatrices();
    GLfloat time = SDL_GetTicks();
    view = cam.getViewMatrix();
    //-----------------
    //std::cout<<to_string(view)<<"\n"
    //-----------------
    MVP = proj*view*model;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawShader->use();

    glUniformMatrix4fv(drawShader->uniform("MVP"), 1, false, glm::value_ptr(MVP));

    glBindVertexArray(drawVAO);
    //inputSource.UploadDepthToTexture();


    //first depthMap
    glBindBuffer(GL_ARRAY_BUFFER, drawVBO);
    glEnableVertexAttribArray(drawShader->attribute("position"));
    glVertexAttribPointer(drawShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(drawVAO);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, bufferWidth*bufferHeight);
    //glDrawArrays(GL_POINTS, 0, bufferWidth*bufferHeight);
    //second depthMap
    //glUniform1i(drawShader->uniform("depthTexture"), 1);
    //glUniformMatrix4fv(drawShader->uniform("MVP"), 1, false, glm::value_ptr(proj*view*localTransform));
    //glUniform3f(drawShader->uniform("shadeColor"), 0, 1, 0);
    //glDrawArrays(GL_POINTS, 0, bufferWidth*bufferHeight);
    //
    mat4 newMVP = proj*view;//*scaleMat

    window.swap();
  }
}

void Application::setupShaders() {
  drawShader = (make_unique<ShaderProgram>());
  drawShader->initFromFiles("shaders/draw.vert", "shaders/draw.frag");
  drawShader->addAttribute("position");
  //drawShader->addAttribute("texCoords");
  //drawShader->addUniform("depthTexture");
  drawShader->addUniform("MVP");
  //drawShader->addUniform("shadeColor");
}

void Application::setupTextures() {
  glGenTextures(1, &leftTex);
  glBindTexture(GL_TEXTURE_2D, leftTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //Texture2
  glGenTextures(1, &rightTex);
  glBindTexture(GL_TEXTURE_2D, rightTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::setupBuffers() {

  for(int i=0;i<bufferWidth;++i){
    for(int j=0;j<bufferHeight;++j){
      rectangle[i*bufferWidth+j] = vec3(i,j, 0);
    }
  }
  //Now build indices
  indices.push_back(0);
  indices.push_back(bufferWidth);
  for(int i=1;i<=bufferHeight;++i){
    for(int j=1;j<bufferWidth;++j){
      indices.push_back((i-1)*bufferWidth + j);
      indices.push_back(i*bufferWidth + j);

      if(j==bufferWidth-1) {
        indices.push_back(i*bufferWidth +j);
        indices.push_back(i*bufferWidth);
        indices.push_back(i*bufferWidth);
        indices.push_back((i+1)*bufferWidth);
      }
    }
  }
  indices.pop_back();
  uint indicesCount = indices.size();

  std::cout<<"Indices Count : "<<indicesCount<<"\n";
  //==========================
  //Create Vertex Array Object
  glGenVertexArrays(1, &drawVAO);
  glBindVertexArray(drawVAO);

  glGenBuffers(1, &drawVBO);
  glBindBuffer(GL_ARRAY_BUFFER, drawVBO);
  glBufferData(GL_ARRAY_BUFFER, bufferWidth * bufferHeight * sizeof(glm::vec3), rectangle.data(), GL_STATIC_DRAW);
  // //Assign attribs
  glEnableVertexAttribArray(drawShader->attribute("position"));
  glVertexAttribPointer(drawShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Bind index buffer
  glGenBuffers(1, &indexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint), indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);	//unbind VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


Application::~Application() {
  stbi_image_free(left);
  stbi_image_free(right);
}
