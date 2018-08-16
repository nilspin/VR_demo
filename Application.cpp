#define STB_IMAGE_IMPLEMENTATION


#include "Application.h"
#include "stb_image.h"
#include<fstream>

SDL_Event event;

Application::Application() {
  //rectangle.resize(bufferWidth*bufferHeight);
  //texCoords.resize(bufferHeight*bufferWidth);
  left = stbi_load("assets/left.jpg", &imgWidth, &imgHeight, &channels, 0);
//  right = stbi_load("assets/right.jpg", &imgWidth, &imgHeight, &channels, 0);
  cout<<"Image width: "<<imgWidth<<"\nImage height: "<<imgHeight<<"\nChannels: "<<channels<<"\n";
  if(left == nullptr) {cout<<"could not read first image file!"<<endl; exit(0);}
//  if(right == nullptr) {cout<<"could not read second image file!"<<endl; exit(0);}
  //cam.setPosition(glm::vec3(320, 240, 300));
  cam.setPosition(glm::vec3(0,0,0));
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  setupTextures();
  setupShaders();
  setupBuffers();
}

void Application::run() {
  while (!quit)
  {
    
    processEvents();
    //First things first
    cam.calcMatrices();
    GLfloat time = SDL_GetTicks();
    view = cam.getViewMatrix();
    //-----------------
    //std::cout<<to_string(view)<<"\n"
    //-----------------
    MVP = proj*view*model;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, leftTex);
    drawShader->use();

    
    glUniform1i(drawShader->uniform("imageTexture"), leftTex);
    glUniformMatrix4fv(drawShader->uniform("MVP"), 1, false, glm::value_ptr(MVP));

    glBindVertexArray(drawVAO);
    //inputSource.UploadDepthToTexture();


    //Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, drawVBO);
    glEnableVertexAttribArray(drawShader->attribute("position"));
    glVertexAttribPointer(drawShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glEnableVertexAttribArray(drawShader->attribute("texCoords"));
    glVertexAttribPointer(drawShader->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glDrawElements(GL_TRIANGLES, realIndices.size(), GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_LINES, realIndices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    window.swap();
  }
}

void Application::setupTextures() {
  glGenTextures(1, &leftTex);
  glBindTexture(GL_TEXTURE_2D, leftTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
  glGenerateMipmap(GL_TEXTURE_2D);
  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D,0);

  //Texture2
  glGenTextures(1, &rightTex);
  glBindTexture(GL_TEXTURE_2D, rightTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
  glGenerateMipmap(GL_TEXTURE_2D);
  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  //TODO: free images here
}

void Application::setupBuffers() {

  int count =0;
  for(float i=-bufferWidth/2.0f;i<(float)bufferWidth/2.0f;++i){
    for(float j=-bufferHeight/2.0f;j<(float)bufferHeight/2.0f;++j){
      float x = (i+(bufferWidth/2.0f))/(float)bufferWidth;
      float y = (j+(bufferHeight/2.0f))/(float)bufferHeight;
      texCoords.push_back(vec2(x,y));
      rectangle.push_back(vec3(i,j, 0));
      //count++;
    }
  }
  cout<<"Number of Vertices: "<<rectangle.size()<<"\n";
  cout<<"Number of textureCoordinates: "<<texCoords.size()<<"\n";
  
   //Now build indices
  indices.push_back(0);
  indices.push_back(bufferWidth);
  for(int i=1;i<bufferHeight;++i){
    for(int j=1;j<bufferWidth;++j){
      indices.push_back((i-1)*bufferWidth + j);
      indices.push_back(i*bufferWidth + j);

      if(i<bufferHeight-1 && j==bufferWidth-1) {
        indices.push_back(i*bufferWidth +j);
        indices.push_back(i*bufferWidth);
        indices.push_back(i*bufferWidth);
        indices.push_back((i+1)*bufferWidth);
      }
    }
  }
  
  uint indicesCount = indices.size();
  cout<<"Total number of triangles: "<<indicesCount<<"\n";
  //Generate actual indices for each triangle
  for(int i=0;i<indices.size()-2;++i)
  {
    realIndices.emplace_back(indices[i]);
    realIndices.emplace_back(indices[i+1]);
    realIndices.emplace_back(indices[i+2]);
  }
  
  
 
  //==========================
  //Create Vertex Array Object
  glGenVertexArrays(1, &drawVAO);
  glBindVertexArray(drawVAO);

  //Position data
  glGenBuffers(1, &drawVBO);
  glBindBuffer(GL_ARRAY_BUFFER, drawVBO);
  glBufferData(GL_ARRAY_BUFFER, bufferWidth * bufferHeight * sizeof(glm::vec3), rectangle.data(), GL_STATIC_DRAW);
  // //Assign attribs
  glEnableVertexAttribArray(drawShader->attribute("position"));
  glVertexAttribPointer(drawShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Texture coordinates
  glGenBuffers(1, &texCoordVBO);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glBufferData(GL_ARRAY_BUFFER, bufferWidth * bufferHeight * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(drawShader->attribute("texCoords"));
  glVertexAttribPointer(drawShader->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);

  //Bind index buffer
  glGenBuffers(1, &indexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, realIndices.size()*sizeof(uint), realIndices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);	//unbind VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Application::setupShaders() {
  drawShader = (make_unique<ShaderProgram>());
  drawShader->initFromFiles("shaders/draw.vert", "shaders/draw.frag");
  drawShader->addAttribute("position");
  drawShader->addAttribute("texCoords");
  drawShader->addUniform("imageTexture");
  drawShader->addUniform("MVP");
  //drawShader->addUniform("shadeColor");
}

void Application::processEvents()
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
}

Application::~Application() {
  stbi_image_free(left);
  stbi_image_free(right);
}
