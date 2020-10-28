#include "LeverHandleActor.h"

#include "HammeringStatics.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ALeverHandleActor::ALeverHandleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	SetRootComponent(SceneRootComponent);

	HandleAttachPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleAttachPoint"));
	// SetRootComponent(HandleAttachPoint);
	HandleAttachPoint->SetupAttachment(GetRootComponent());
	HandleAttachPoint->SetMobility(EComponentMobility::Movable);
	HandleAttachPoint->SetSimulatePhysics(true);
	HandleAttachPoint->SetCollisionProfileName("PhysicsActor", true);
	HandleAttachPoint->SetMassOverrideInKg(NAME_None,0.5f);
	HandleAttachPoint->SetCollisionObjectType(ECC_Grabbable);
	HandleAttachPoint->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HandleAttachPoint->SetGenerateOverlapEvents(true);
	HandleAttachPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
	HandleAttachPoint->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	HandleAttachPoint->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	HandleAttachPoint->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	HandleAttachPoint->SetCollisionResponseToChannel(ECC_Grabbable, ECR_Block);
	HandleAttachPoint->SetSimulatePhysics(true);
	HandleAttachPoint->OnComponentBeginOverlap.AddDynamic(this, &ALeverHandleActor::OnAttachPointBeingOverlapped);

	
	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
	HandleMesh->SetupAttachment(GetRootComponent());
	HandleMesh->SetMobility(EComponentMobility::Movable);
	HandleMesh->SetSimulatePhysics(true);
	HandleMesh->SetCollisionProfileName("PhysicsActor", true);
	HandleMesh->SetMassOverrideInKg(NAME_None,0.5f);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetMobility(EComponentMobility::Static);
	
	PhysicsConstraintMain = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintMain"));
	PhysicsConstraintMain->SetupAttachment(GetRootComponent());
	PhysicsConstraintMain->SetWorldLocation(HandleMesh->GetComponentLocation());
	PhysicsConstraintMain->SetRelativeRotation(FRotator(90,0,0));
	PhysicsConstraintMain->SetDisableCollision(true);
	PhysicsConstraintMain->SetConstrainedComponents(BaseMesh, NAME_None, HandleAttachPoint, NAME_None);
	PhysicsConstraintMain->SetAngularSwing1Limit(ACM_Limited,45);
	PhysicsConstraintMain->SetAngularSwing2Limit(ACM_Locked,45);
	PhysicsConstraintMain->SetAngularTwistLimit(ACM_Locked,0);
	PhysicsConstraintMain->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhysicsConstraintMain->SetAngularVelocityDriveTwistAndSwing(false, true);
	PhysicsConstraintMain->SetAngularOrientationDrive(true, false);
	PhysicsConstraintMain->SetAngularOrientationTarget(FRotator(0,0,-40));
	PhysicsConstraintMain->SetAngularDriveParams(5000,500,0);
	
	PhysicsConstraintVisual = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintVisual"));
	PhysicsConstraintVisual->SetupAttachment(GetRootComponent());
	PhysicsConstraintVisual->SetConstrainedComponents(HandleAttachPoint, NAME_None, HandleMesh, NAME_None);
	PhysicsConstraintVisual->SetDisableCollision(true);
	PhysicsConstraintVisual->SetLinearXLimit(LCM_Locked,0);
	PhysicsConstraintVisual->SetLinearYLimit(LCM_Locked,0);
	PhysicsConstraintVisual->SetLinearZLimit(LCM_Locked,0);
	PhysicsConstraintVisual->SetAngularSwing1Limit(ACM_Locked,0);
	PhysicsConstraintVisual->SetAngularSwing2Limit(ACM_Locked,0);
	PhysicsConstraintVisual->SetAngularTwistLimit(ACM_Locked,0);

	PhysicsConstraintGrab = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintGrab"));
	PhysicsConstraintGrab->SetupAttachment(GetRootComponent());
	PhysicsConstraintGrab->SetAngularSwing1Limit(ACM_Limited, 15.0f);
	PhysicsConstraintGrab->SetAngularSwing2Limit(ACM_Limited, 10.0f);
	PhysicsConstraintGrab->SetAngularTwistLimit(ACM_Locked, 0.0f);
	PhysicsConstraintGrab->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhysicsConstraintGrab->SetAngularVelocityDriveTwistAndSwing(false, true);
	PhysicsConstraintGrab->SetAngularOrientationDrive(true, false);
	PhysicsConstraintGrab->SetAngularDriveParams(2500.0f, 100.0f, 0.0f);
}

void ALeverHandleActor::GrabPressed(USceneComponent* AttachTo)
{
	if (bSnapToHand)
	{
		FVector AttachLocation = AttachTo->GetComponentLocation();
		FRotator AttachRotation = AttachTo->GetComponentRotation();

		if (bUseCustomLocation)
		{
			const FVector DeltaLocation = GetCustomAttachLocation() - HandleAttachPoint->GetComponentLocation();

			AttachLocation -= DeltaLocation;
		}
		if (bUseCustomRotation)
		{
			const FRotator DeltaRotation = GetCustomAttachRotation() - HandleAttachPoint->GetComponentRotation();
			AttachRotation -= DeltaRotation;
		}
		SetActorLocationAndRotation(AttachLocation, AttachRotation);
	}

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Green, FString::Printf(TEXT("Object is found: %s"), *GetName()));
	}


	if (!HandleAttachPoint->IsSimulatingPhysics())
	{
		HandleAttachPoint->SetSimulatePhysics(true);
	}

	PhysicsConstraintGrab->ConstraintActor1 = this;
	PhysicsConstraintGrab->ConstraintActor2 = AttachTo->GetOwner();

	// Note: Static Mesh Component should be the root component otherwise during simulation it will detach from root
	PhysicsConstraintGrab->SetConstrainedComponents(HandleAttachPoint, NAME_None,
                                                         Cast<UPrimitiveComponent>(AttachTo), NAME_None);
	
	PhysicsConstraintMain->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	bIsTicking = true;
}

void ALeverHandleActor::GrabReleased()
{
	PhysicsConstraintGrab->BreakConstraint();
	bIsTicking = false;
}

int ALeverHandleActor::GetGrabType()
{
	return TypeOfGrab;
}

void ALeverHandleActor::BeginPlay()
{
	Super::BeginPlay();

	InitialAttachPointRotation = HandleAttachPoint->GetComponentRotation();
	LastStillRotationAttachPoint = InitialAttachPointRotation;
	if(bHasStillPosition)
	{
		PhysicsConstraintMain->SetAngularDriveMode(EAngularDriveMode::SLERP);
	}
}

FVector ALeverHandleActor::GetCustomAttachLocation() const
{
	return CustomAttachPoint->GetComponentLocation();
}

FRotator ALeverHandleActor::GetCustomAttachRotation() const
{
	return CustomAttachPoint->GetComponentRotation();
}

void ALeverHandleActor::OnAttachPointBeingOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Object is found: %s"), *OtherActor->GetName());
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Green, FString::Printf(TEXT("Object is found: %s"), *OtherActor->GetName()));
	}
}

void ALeverHandleActor::CheckForDetachAfterMaximumRotation()
{
	auto CurrentDifferentiateRotation = ((HandleAttachPoint->GetComponentRotation() - InitialAttachPointRotation).Euler().Size());
	auto StillDifferentialRotation = ((HandleAttachPoint->GetComponentRotation() - LastStillRotationAttachPoint).Euler().Size());
	if(bIsDebugHandle)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,20.0f, FColor::Green, FString::Printf(TEXT("CurrentRotation: %f"), StillDifferentialRotation));
		}
	}

	if(CurrentDifferentiateRotation > MaxRotation && StillDifferentialRotation > MinRotationGapForStillStage)
	{
		if(bHasStillPosition)
		{
			PhysicsConstraintMain->SetAngularDriveMode(EAngularDriveMode::SLERP);
		}

		LastStillRotationAttachPoint = HandleAttachPoint->GetComponentRotation();
		GrabReleased();
	}
}

void ALeverHandleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsTicking){return;}

	CheckForDetachAfterMaximumRotation();
	
}

