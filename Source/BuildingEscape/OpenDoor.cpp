

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	MaxYawRotation = 70.f;
	YawRotationValue = 2.f;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//auto Owner = GetOwner();
	//FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);

	//Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (TickType != ELevelTick::LEVELTICK_PauseTick) {
		auto Owner = GetOwner();
		float CurrentYaw = Owner->GetActorRotation().Yaw;
		if (CurrentYaw < MaxYawRotation) {

			FRotator NewRotation = FRotator(0.0f, CurrentYaw + YawRotationValue, 0.0f);

			Owner->SetActorRotation(NewRotation);
		}
	}
}

