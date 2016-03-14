

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	Player = GetWorld()->GetFirstPlayerController();
	FString ObjectName = Owner->GetName();
	FString ObjectPos = Owner->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos);

	// ...
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector Location;
	FRotator rotator;
	Player->GetPlayerViewPoint(Location, rotator);
	///UE_LOG(LogTemp, Warning, TEXT("location: %s, rot: %s"), *location.ToString(), *rotator.ToString());
	FVector LineTraceEnd = Location + rotator.Vector() * Reach;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, Owner);

	/// Draw red trace in the world
	FHitResult LineHit;
	DrawDebugLine(GetWorld(), Location, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
	bool DidHit = GetWorld()->LineTraceSingleByObjectType(
		LineHit,
		Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (DidHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *LineHit.Actor->GetName());
	}
}

