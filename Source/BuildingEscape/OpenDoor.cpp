

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
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (TickType != ELevelTick::LEVELTICK_PauseTick) {
		// Poll the Trigger Volume to see if should trigger
		if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {  // update to set flag
			IsOpening = true;
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
		if (IsOpening && GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
			IsClosing = true, IsOpening = false;
		}
	}
	if (IsOpening) {
		OpenDoor();
	} else if (IsClosing) {
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor() {
	float CurrentYaw = Owner->GetActorRotation().Yaw;
	if (CurrentYaw < MaxOpenAngle) {

		FRotator NewRotation = FRotator(0.0f, CurrentYaw + OpenByValue, 0.0f);

		Owner->SetActorRotation(NewRotation);
	}
}

void UOpenDoor::CloseDoor() {
	float CurrentYaw = Owner->GetActorRotation().Yaw;
	if (CurrentYaw > 0) {
		Owner->SetActorRotation(FRotator(0.f, CurrentYaw - OpenByValue, 0.f));
	}
}

