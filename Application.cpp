#define STB_IMAGE_IMPLEMENTATION


#include "Application.h"
#include "stb_image.h"
#include<fstream>

SDL_Event event;

Application::Application() {
  cam1.setPosition(glm::vec3(-0.032,0,0));  //Set eye 32mm left of origin
  cam2.setPosition(glm::vec3(0.032,0,0)); //Set eye 32mm right or origin
  //Load images from disk
  left = stbi_load("assets/left.jpg", &imgWidth, &imgHeight, &channels, 0);
  right = stbi_load("assets/right.jpg", &imgWidth, &imgHeight, &channels, 0);
  cout<<"Image width: "<<imgWidth<<"\nImage height: "<<imgHeight<<"\nChannels: "<<channels<<"\n";
  if(left == nullptr) {cout<<"could not read first image file!"<<endl; exit(0);}
  if(right == nullptr) {cout<<"could not read second image file!"<<endl; exit(0);}
  //Set proper GL state
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  //Init functions
  setupTextures();
  setupShaders();
  setupBuffers();
}

void Application::run() {
  while (!quit)
  {

    processEvents();
    
    //First things first
    cam1.calcMatrices();
    cam2.calcMatrices();
    view_cam1 = cam1.getViewMatrix();
    view_cam2 = cam2.getViewMatrix();

    GLfloat time = SDL_GetTicks();
    //-----------------
    //std::cout<<to_string(view)<<"\n"
    //-----------------
    MVP_cam1 = proj*view_cam1*model;
    MVP_cam2 = proj*view_cam2*model;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Draw Left view
    glViewport(0,0,1920/2,960);
    
    glUniform1i(drawShader->uniform("imageTexture"), 0);  //Our left-texture is already attached at unit 0
    glUniformMatrix4fv(drawShader->uniform("MVP"), 1, false, glm::value_ptr(MVP_cam1));
    glBindVertexArray(drawVAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glDrawElements(GL_TRIANGLES, realIndices.size(), GL_UNSIGNED_INT, 0);
    
    //Draw right view
    glViewport(1920/2,0,960,960);
    glUniform1i(drawShader->uniform("imageTexture"), 1);  //Our right-texture is already attached at unit 1
    glUniformMatrix4fv(drawShader->uniform("MVP"), 1, false, glm::value_ptr(MVP_cam2));
      
    glDrawElements(GL_TRIANGLES, realIndices.size(), GL_UNSIGNED_INT, 0);
    
    window.swap();
  }
}

void Application::setupTextures() {
  //Upload left texture
  glGenTextures(1, &leftTex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, leftTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
  
  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glGenerateMipmap(GL_TEXTURE_2D);
  //glBindTexture(GL_TEXTURE_2D,0);

  //Upload right texture
  glGenTextures(1, &rightTex);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, rightTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, right);

  //filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glGenerateMipmap(GL_TEXTURE_2D);
  //glBindTexture(GL_TEXTURE_2D, 0);

  //Loading done. Now free images.
  stbi_image_free(left);
  stbi_image_free(right);
}

void Application::setupBuffers() {

  //Fill up position and texture-coordinate arrays
  int count =0;
  for(float i=0;i<=bufferWidth;++i){
    for(float j=0;j<=bufferHeight;++j){
      float x = i/(float)bufferWidth;
      float y = j/(float)bufferHeight;
      texCoords.push_back(vec2(x,y));
      rectangle.push_back(vec3(i-(float)bufferWidth/2.0f,j-(float)bufferHeight/2.0f, -5.0f));
    }
  }
  cout<<"Number of Vertices: "<<rectangle.size()<<"\n";
  cout<<"Number of textureCoordinates: "<<texCoords.size()<<"\n";
  
  //Now build indices
  bufferHeight+=1; bufferWidth+=1;
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
  
  
  //Now upload to device-side buffers
  //==========================
  //Create Vertex Array Object
  glGenVertexArrays(1, &drawVAO);
  glBindVertexArray(drawVAO);

  //Position data
  glGenBuffers(1, &drawVBO);
  glBindBuffer(GL_ARRAY_BUFFER, drawVBO);
  glBufferData(GL_ARRAY_BUFFER, bufferWidth * bufferHeight * sizeof(glm::vec3), rectangle.data(), GL_STATIC_DRAW);
  // Assign attribs
  glEnableVertexAttribArray(drawShader->attribute("position"));
  glVertexAttribPointer(drawShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(cylinderTransformShader->attribute("position"));
  glVertexAttribPointer(cylinderTransformShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(barrelProjShader->attribute("position"));
  glVertexAttribPointer(barrelProjShader->attribute("position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Texture coordinates
  glGenBuffers(1, &texCoordVBO);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glBufferData(GL_ARRAY_BUFFER, bufferWidth * bufferHeight * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(drawShader->attribute("texCoords"));
  glVertexAttribPointer(drawShader->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(cylinderTransformShader->attribute("texCoords"));
  glVertexAttribPointer(cylinderTransformShader->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(barrelProjShader->attribute("texCoords"));
  glVertexAttribPointer(barrelProjShader->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);

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
  drawShader->initFromFiles("shaders/passthrough.vert", "shaders/passthrough.frag");
  drawShader->addAttribute("position");
  drawShader->addAttribute("texCoords");
  drawShader->addUniform("imageTexture");
  drawShader->addUniform("MVP");

  cylinderTransformShader = (make_unique<ShaderProgram>());
  cylinderTransformShader->initFromFiles("shaders/cylinder.vert", "shaders/passthrough.frag");
  cylinderTransformShader->addAttribute("position");
  cylinderTransformShader->addAttribute("texCoords");
  cylinderTransformShader->addUniform("imageTexture");
  cylinderTransformShader->addUniform("MVP");

  barrelProjShader = (make_unique<ShaderProgram>());
  barrelProjShader->initFromFiles("shaders/cylinder.vert", "shaders/barrelDist.frag");
  barrelProjShader->addAttribute("position");
  barrelProjShader->addAttribute("texCoords");
  barrelProjShader->addUniform("imageTexture");
  barrelProjShader->addUniform("MVP");
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
            cam1.move(FORWARD);
            cam2.move(FORWARD);
            break;

          case SDLK_s:
            cam1.move(BACK);
            cam2.move(BACK);
            break;

          case SDLK_a:
            cam1.move(LEFT);
            cam2.move(LEFT);
            break;

          case SDLK_d:
            cam1.move(RIGHT);
            cam2.move(RIGHT);
            break;

          case SDLK_UP:
            cam1.move(UP);
            cam2.move(UP);
            break;

          case SDLK_DOWN:
            cam1.move(DOWN);
            cam2.move(DOWN);
            break;

          case SDLK_LEFT:
            cam1.move(ROT_LEFT);
            cam2.move(ROT_LEFT);
            break;

          case SDLK_RIGHT:
            cam1.move(ROT_RIGHT);
            cam2.move(ROT_RIGHT);
            break;

          case SDLK_r:
            cam1.reset();
            cam2.reset();
            std::cout << "[R]Camera reset. \n";
            break;
            
          //Key-bindings to switch shaders
          case SDLK_c:
            cylinderTransformShader->use();
            std::cout<<"[C]Switched to cylinder view shader. \n";
            break;

          case SDLK_b:
            barrelProjShader->use();
            std::cout<<"[B]Switched to barrel distortion shader.\n";
            break;

          case SDLK_v:
            drawShader->use();
            std::cout<<"[V]Switched to regular quad shader. \n";
            break;
        }
        break;

      case SDL_MOUSEMOTION:
        cam1.rotate();
        cam2.rotate();
        break;
    }
  }
}
