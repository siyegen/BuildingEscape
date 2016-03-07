

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	MaxOpenAngle = 70.f;
	OpenByValue = 2.f;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (TickType != ELevelTick::LEVELTICK_PauseTick) {
		// Poll the Trigger Volume to see if should trigger
		if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
			OpenDoor();
		}
	}
}

void UOpenDoor::OpenDoor() {
	auto Owner = GetOwner();
	float CurrentYaw = Owner->GetActorRotation().Yaw;
	if (CurrentYaw < MaxOpenAngle) {

		FRotator NewRotation = FRotator(0.0f, CurrentYaw + OpenByValue, 0.0f);

		Owner->SetActorRotation(NewRotation);
	}
}

