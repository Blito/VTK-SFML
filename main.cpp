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

// SFML INCLUDES
#include <SFML/Graphics.hpp>

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

int main()
{
	// ########### SETUP SFML ########### 
    sf::RenderWindow window;
    window.create(sf::VideoMode(sfmlWidth, sfmlHeight), "SFML Window");

    // Create texture buffer
    texture_raw = new unsigned char[textureWidth * textureHeight * 4]; // RGBA
    for (unsigned int i = 0; i < textureWidth * textureHeight * 4; i+=4)
    {
    	texture_raw[i] = 255; // red square
    	texture_raw[i+3] = 255;
    }

    // Create texture
    sf::Texture texture;
    texture.create(textureWidth, textureHeight);
    texture.update(texture_raw);

    // Create sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);
    unsigned int x = 50;
    unsigned int y = 100;
    sprite.setPosition(x, y);

    // Create FPS output
    sf::Text fpsOutput;
    sf::Font font;
    if (!font.loadFromFile(fontPath))
        return -1;
    fpsOutput.setFont(font);
    fpsOutput.setCharacterSize(24);
    fpsOutput.setColor(sf::Color::Red);
    fpsOutput.setPosition(0, sfmlHeight-30);
	// ########### END SETUP SFML ###########

	// ########### SETUP VTK ###########
	auto renderer = vtkRenderer::New();
    renderer->SetBackground(.05,.1,.15);

	// Create render window
    auto renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(600,600);

    // Create a camera
    auto camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition(50, 50, 100);
    camera->SetFocalPoint(0, 0, 0);
    renderer->SetActiveCamera(camera);

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
    renderer->ResetCamera();
    // ########### END SETUP VTK ###########

    auto startTime = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    while (true)
    {
	    // ########### UPDATE VTK ###########
		texturedPlane->SetOrientation(0,0,0);
	    texturedPlane->RotateY(rot_x++);
	    texturedPlane->RotateX(rot_y--);
	    texturedPlane->RotateZ(rot_z++);

	    renWin->Render();
	    // ########### END UPDATE VTK ###########

	    // ########### UPDATE SFML ###########
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
		// ########### END UPDATE SFML ###########
	}
}