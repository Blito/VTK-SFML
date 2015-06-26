// STL includes
#include <iostream>
#include <chrono>
#include <sstream>

// VTK INCLUDES
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

// SFML INCLUDES
#include <SFML/Graphics.hpp>

#include <thread>

// VTK variables
constexpr float planeWidth = 200.0f;
constexpr float planeHeight = 100.0f;
float rot_x = 0;
float rot_y = 0;
float rot_z = 0;

// SFML variables
constexpr unsigned int sfmlWidth = 800;
constexpr unsigned int sfmlHeight = 600;
unsigned char * texture_raw;
constexpr unsigned int textureWidth = 50;
constexpr unsigned int textureHeight = 100;
constexpr char * fontPath = "arial.ttf";

void sfml()
{
    // Setup
    sf::RenderWindow window;
    window.create(sf::VideoMode(sfmlWidth, sfmlHeight), "SFML Window");

    // Create texture buffer
    texture_raw = new unsigned char[textureWidth * textureHeight * 4]; // RGBA
    for (unsigned int i = 0; i < textureWidth * textureHeight * 4; i++)
    {
        texture_raw[i] = 255; // red square
    }

    // Create texture
    sf::Texture texture;
    texture.create(textureWidth, textureHeight);
    texture.update(texture_raw);

    // Create sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);
    unsigned int x = 150;
    unsigned int y = 30;
    sprite.setPosition(x, y);

    // Create FPS output
    sf::Text fpsOutput;
    sf::Font font;
    if (!font.loadFromFile(fontPath))
        return;
    fpsOutput.setFont(font);
    fpsOutput.setCharacterSize(24);
    fpsOutput.setColor(sf::Color::Red);
    fpsOutput.setPosition(0, sfmlHeight-30);

    // Render
    auto startTime = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    while (true)
    {
        // Event loop
        sf::Event event;
        while (window.pollEvent(event)) {}

        // Update FPS
        endTime = std::chrono::high_resolution_clock::now();
        double fps = (double)1000.0/std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        std::ostringstream strs;
        strs << "FPS: " << fps;
        std::string str = strs.str();
        fpsOutput.setString(str);
        startTime = std::chrono::high_resolution_clock::now();

        // Update square
        sprite.setPosition(x, y);
        x < sfmlWidth - textureWidth ? x++ : x = 0;
        y < sfmlHeight - textureHeight ? y++ : y = 0;

        window.clear();
        window.draw(fpsOutput);
        window.draw(sprite);
        window.display();
    }
}

void vtk()
{
    // Setup
    auto renderer = vtkRenderer::New();

    // Create render window
    auto renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(600,600);

    auto renderWindowInteractor = vtkRenderWindowInteractor::New();
    renderWindowInteractor->SetRenderWindow(renWin);

    // Create a camera
    auto camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition(50, 50, 100);
    camera->SetFocalPoint(0, 0, 0);
    renderer->SetActiveCamera(camera);
    renderer->SetBackground(.05,.1,.15);

    // Create a plane
    auto texturedPlane = vtkActor::New();
    auto plane = vtkSmartPointer<vtkPlaneSource>::New();

    plane->SetOrigin(0, planeHeight, 0);
    plane->SetPoint1(planeWidth, planeHeight, 0);
    plane->SetPoint2(0, 0, 0);

    auto planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(plane->GetOutputPort());

    texturedPlane->SetMapper(planeMapper);

    texturedPlane->SetOrigin(planeWidth / 2,
                             planeHeight, 0);

    renderer->AddActor(texturedPlane);

    // Create a plane
    auto texturedPlane2 = vtkActor::New();
    auto plane2 = vtkSmartPointer<vtkPlaneSource>::New();

    plane2->SetOrigin(0, planeHeight, 0);
    plane2->SetPoint1(planeWidth, 0, planeHeight);
    plane2->SetPoint2(0, 0, 0);

    auto planeMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper2->SetInputConnection(plane2->GetOutputPort());

    texturedPlane2->SetMapper(planeMapper2);

    texturedPlane2->SetOrigin(planeWidth / 2,
                             planeHeight, 0);

    renderer->AddActor(texturedPlane2);

    renderer->ResetCamera();

    auto style = vtkInteractorStyleTrackballCamera::New(); 
    renderWindowInteractor->SetInteractorStyle( style );

    renderWindowInteractor->Start();

    // Render
    while(true)
    {
        texturedPlane->SetOrientation(0,0,0);
        texturedPlane->RotateY(rot_x++);
        texturedPlane->RotateX(rot_y--);
        texturedPlane->RotateZ(rot_z++);
            
        renWin->Render();
    }
}

int main()
{
    std::thread sfml_thread(sfml);
    std::thread vtk_thread(vtk);

    sfml_thread.join();
    vtk_thread.join();
}