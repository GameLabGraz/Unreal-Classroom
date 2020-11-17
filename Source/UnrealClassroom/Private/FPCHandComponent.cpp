#include "FPCHandComponent.h"
#include "Components/SphereComponent.h"

UFPCHandComponent::UFPCHandComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HandMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMeshComponent"));
	//HandMeshComponent->SetupAttachment(GetAttachmentRoot());
	

	GrabSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphereComponent"));
	GrabSphereComponent->SetupAttachment(HandMeshComponent);
}


void UFPCHandComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UFPCHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

