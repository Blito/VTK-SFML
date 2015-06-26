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

// VTK variables
constexpr float planeWidth = 200.0f;
constexpr float planeHeight = 100.0f;

int main()
{
    auto renderer = vtkRenderer::New();

    // Create render window
    auto renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(600,600);

    // Create a plane
    auto texturedPlane = vtkActor::New();
    auto plane = vtkPlaneSource::New();
    plane->SetOrigin(0, planeHeight, 0);
    plane->SetPoint1(planeWidth, planeHeight, 0);
    plane->SetPoint2(0, 0, 0);

    auto planeMapper = vtkPolyDataMapper::New();
    planeMapper->SetInputConnection(plane->GetOutputPort());
    texturedPlane->SetMapper(planeMapper);
    texturedPlane->SetOrigin(planeWidth / 2, planeHeight, 0);

    renderer->AddActor(texturedPlane);
    renderer->ResetCamera();

    // Create a RenderWindowInteractor
    auto renderWindowInteractor = vtkRenderWindowInteractor::New();
    renderWindowInteractor->SetRenderWindow(renWin);
    renderWindowInteractor->Start();

    // Render
    float rot = 0.0f;
    while(true)
    {
        texturedPlane->SetOrientation(0,0,0);
        texturedPlane->RotateZ(rot++);
            
        renWin->Render();
    }
}