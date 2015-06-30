// VTK INCLUDES
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCommand.h>

// VTK variables
constexpr float planeWidth = 200.0f;
constexpr float planeHeight = 100.0f;
vtkActor * texturedPlane;
vtkRenderWindowInteractor * renderWindowInteractor;
vtkRenderWindow * renWin;
float rot = 0.0f;

class RotateCommand : public vtkCommand
{
public:
    vtkTypeMacro(RotateCommand, vtkCommand);

    static RotateCommand * New()
    {
        return new RotateCommand;
    }

    void Execute(vtkObject * vtkNotUsed(caller),
                 unsigned long vtkNotUsed(eventId), 
                 void * vtkNotUsed(callData))
    {
        texturedPlane->SetOrientation(0,0,0);
        texturedPlane->RotateZ(rot++);
            
        renWin->Render();
    }
};

int main()
{
    auto renderer = vtkRenderer::New();

    // Create render window
    renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    renWin->SetSize(600,600);

    // Create a plane
    texturedPlane = vtkActor::New();
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
    renderWindowInteractor = vtkRenderWindowInteractor::New();
    renderWindowInteractor->SetRenderWindow(renWin);
    renderWindowInteractor->Initialize();
    renderWindowInteractor->CreateRepeatingTimer(1);
    RotateCommand * rotateCallback =  RotateCommand::New();
    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, rotateCallback );

    renderWindowInteractor->Start();
}