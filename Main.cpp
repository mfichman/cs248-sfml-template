#include <SFML/Window.hpp>
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <memory>
#include <iostream>

#define MODEL_PATH "models/bunny.obj"
#define ZNEAR 0.1f
#define ZFAR 500.0f

// Note: See the SMFL documentation for info on setting up fullscreen mode
// and using rendering settings
// http://www.sfml-dev.org/tutorials/1.6/window-window.php
sf::WindowSettings settings(24, 8, 2);
sf::Window window(sf::VideoMode(800, 600), "CS248 Rules!", sf::Style::Close, settings);
 
// This is a clock you can use to control animation.  For more info, see:
// http://www.sfml-dev.org/tutorials/1.6/window-time.php
sf::Clock clck;

// This creates an asset importer using the Open Asset Import library.
// It automatically manages resources for you, and frees them when the program
// exits.
Assimp::Importer importer;
const aiScene* scene;
std::vector<unsigned> indexBuffer;

void initOpenGL();
void loadAssets();
void handleInput();
void renderFrame();

int main(int argc, char** argv) {

    initOpenGL();
    loadAssets();

    // Put your game loop here (i.e., render with OpenGL, update animation)
    while (window.IsOpened()) {

        handleInput();
        renderFrame();
        window.Display();
    }

    return 0;
}




void initOpenGL() {
    // This initializes OpenGL with some common defaults.  More info here:
    // http://www.sfml-dev.org/tutorials/1.6/window-opengl.php
    glClearDepth(1.0f);
    glClearColor(0.15f, 0.15f, 0.15f, 0.15f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthMask(GL_TRUE);

    glEnableClientState(GL_VERTEX_ARRAY);

    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, window.GetWidth()/window.GetHeight(), ZNEAR, ZFAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -5);
}




void loadAssets() {
    // Read in an asset file, and do some post-processing.  There is much 
    // more you can do with this asset loader, including load textures.
    // More info is here:
    // http://assimp.sourceforge.net/lib_html/usage.html
    scene = importer.ReadFile(MODEL_PATH,  
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcessPreset_TargetRealtime_Quality);

    if (!scene || scene->mNumMeshes <= 0) {
        std::cerr << importer.GetErrorString() << std::endl;
        exit(-1);
    }

    // Set up the index buffer.  Each face should have 3 vertices since we
    // specified aiProcess_Triangulate
    aiMesh* mesh = scene->mMeshes[0];
    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indexBuffer.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    std::cout << "Vertices: " << mesh->mNumVertices << std::endl;
    std::cout << "Faces: " << mesh->mNumFaces << std::endl;
    std::cout << "Normals: " << mesh->HasNormals() << std::endl;
    std::cout << "Tex coords: " << mesh->HasTextureCoords(0) << std::endl;
    std::cout << "Indices: " << indexBuffer.size() << std::endl;
}




void handleInput() {
    // Event loop, for processing user input, etc.  For more info, see:
    // http://www.sfml-dev.org/tutorials/1.6/window-events.php
    sf::Event evt;
    while (window.GetEvent(evt)) {
        switch (evt.Type) {
        case sf::Event::Closed: 
            // Close the window.  This will cause the game loop to exit,
            // because the IsOpened() function will no longer return true.
            window.Close(); 
            break;
        case sf::Event::Resized: 
            // If the window is resized, then we need to change the perspective
            // transformation and viewport
            glViewport(0, 0, evt.Size.Width, evt.Size.Height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0f, evt.Size.Width/evt.Size.Height, ZNEAR, ZFAR);
            glMatrixMode(GL_MODELVIEW);
            break;
        }
    }
}





void renderFrame() {
    // Always clear the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Add a little rotation, using the elapsed time for smooth animation
    float elapsed = clck.GetElapsedTime();
    clck.Reset();
    glRotatef(20*elapsed, 0, 1, 0);

    // Just render the first mesh in the imported scene file
    aiMesh* mesh = scene->mMeshes[0];

    // Set the material
    GLfloat material[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.0f);

    // Give OpenGL the arrays of vertices, normals, and texture coordinates
    glVertexPointer(3, GL_FLOAT, sizeof(aiVector3D), mesh->mVertices);
    if (mesh->HasNormals()) {
        glNormalPointer(GL_FLOAT, sizeof(aiVector3D), mesh->mNormals);
        glEnableClientState(GL_NORMAL_ARRAY);
    }
    if (mesh->HasTextureCoords(0)) {
        glTexCoordPointer(2, GL_FLOAT, 0, mesh->mTextureCoords[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    // Draw the model
    glDrawElements(GL_TRIANGLES,  3* mesh->mNumFaces, GL_UNSIGNED_INT, &indexBuffer[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}