
#pragma once

#include "Object.h"
#include "../Spacecrafts/FlareBomb.h"
#include "FlareAsteroid.h"
#include "FlareSimulatedSector.generated.h"

class UFlareSimulatedSpacecraft;
class UFlareGame;
struct FFlarePlayerSave;

/** Sector knowledge status */
UENUM()
namespace EFlareSectorKnowledge
{
    enum Type
    {
        Unknown, /** The exisitaece of this sector is unknown */
        Known, /** The sector is visible on the map but its content is unknown */
        Visited /** The sector is visited, all static structure are visible */
    };
}



/** Sector description */
USTRUCT()
struct FFlareSectorDescription
{
	GENERATED_USTRUCT_BODY()

	/** Name */
	UPROPERTY(EditAnywhere, Category = Sector)
	FString Name;

	/** Sector identifier */
	UPROPERTY(EditAnywhere, Category = Sector)
	FName Identifier;

	/** Orbit phase */
	UPROPERTY(EditAnywhere, Category = Sector)
	float Phase;

	/** Peaceful sector */
	UPROPERTY(EditAnywhere, Category = Sector)
	bool Peaceful;
};

/** Sector orbit description */
USTRUCT()
struct FFlareSectorOrbitDescription
{
	GENERATED_USTRUCT_BODY()

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Sector)
	float Altitude;


	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Sector)
	TArray<FFlareSectorDescription> Sectors;
};


/** Sector celestial body description */
USTRUCT()
struct FFlareSectorCelestialBodyDescription
{
	GENERATED_USTRUCT_BODY()
	/** Parent celestial body identifier */
	UPROPERTY(EditAnywhere, Category = Sector)
	FName CelestialBodyIdentifier;


	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Sector)
	TArray<FFlareSectorOrbitDescription> Orbits;
};


/** Sector orbit parameters */
USTRUCT()
struct FFlareSectorOrbitParameters
{
	GENERATED_USTRUCT_BODY()
	/** Parent celestial body identifier */
	UPROPERTY(EditAnywhere, Category = Save)
	FName CelestialBodyIdentifier;

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Save)
	float Altitude;

	/** Orbit phase */
	UPROPERTY(EditAnywhere, Category = Save)
	float Phase;
};



/** Sector save data */
USTRUCT()
struct FFlareSectorSave
{
    GENERATED_USTRUCT_BODY()

	/** Given Name */
    UPROPERTY(EditAnywhere, Category = Save)
	FString GivenName;

	/** Sector identifier */
    UPROPERTY(EditAnywhere, Category = Save)
    FName Identifier;

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareSpacecraftSave> ShipData;

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareSpacecraftSave> StationData;

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareBombSave> BombData;

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareAsteroidSave> AsteroidData;

	UPROPERTY(VisibleAnywhere, Category = Save)
	TArray<FName> FleetIdentifiers;
};

UCLASS()
class FLARE_API UFlareSimulatedSector : public UObject
{
    GENERATED_UCLASS_BODY()

public:

    /*----------------------------------------------------
        Save
    ----------------------------------------------------*/

	/** Load the sector from a save file */
	virtual void Load(const FFlareSectorDescription* Description, const FFlareSectorSave& Data, const FFlareSectorOrbitParameters& OrbitParameters);

	/** Save the sector to a save file */
    virtual FFlareSectorSave* Save();

    /** Spawn a simulated spacecraft from save data */
    virtual UFlareSimulatedSpacecraft* LoadSpacecraft(const FFlareSpacecraftSave& SpacecraftData);

    /*----------------------------------------------------
        Gameplay
    ----------------------------------------------------*/

    /** Create a station in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateStation(FName StationClass, UFlareCompany* Company, FVector TargetPosition, FRotator TargetRotation = FRotator::ZeroRotator);

    /** Create a ship in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateShip(FName ShipClass, UFlareCompany* Company, FVector TargetPosition);

	/** Create a ship or station in the level  for a specific company. No null parameter accepted */
	UFlareSimulatedSpacecraft* CreateShip(FFlareSpacecraftDescription* ShipDescription, UFlareCompany* Company, FVector TargetLocation, FRotator TargetRotation = FRotator::ZeroRotator);

	/** Add the fleet and its ships from the sector */
	void AddFleet(UFlareFleet* Fleet);

	/** Remove the fleet but let its ships in the sector */
	void DisbandFleet(UFlareFleet* Fleet);

	/** Retire the fleet and its ships from the sector */
	void RetireFleet(UFlareFleet* Fleet);



    /** Init world with empty scenario */
	//virtual void InitEmptyScenario(FFlarePlayerSave* PlayerData);

    /** Init world with peaceful scenario */
	//virtual void InitPeacefulScenario(FFlarePlayerSave* PlayerData);

    /** Init world with threatened scenario */
	//virtual void InitThreatenedScenario(FFlarePlayerSave* PlayerData, UFlareCompany* PlayerCompany);

    /** Init world with aggresve scenario */
	//virtual void InitAggresiveScenario(FFlarePlayerSave* PlayerData, UFlareCompany* PlayerCompany);



protected:

    /*----------------------------------------------------
        Protected data
    ----------------------------------------------------*/

    // Gameplay data
	FFlareSectorSave                        SectorData;

	UPROPERTY()
    TArray<UFlareSimulatedSpacecraft*>      SectorStations;

	UPROPERTY()
    TArray<UFlareSimulatedSpacecraft*>      SectorShips;

	UPROPERTY()
	TArray<UFlareFleet*>                    SectorFleets;

	AFlareGame*                             Game;
	FFlareSectorOrbitParameters             SectorOrbitParameters;
	const FFlareSectorDescription*                SectorDescription;

public:

    /*----------------------------------------------------
        Getters
    ----------------------------------------------------*/

    AFlareGame* GetGame() const
    {
		return Game;
    }

    inline FName GetIdentifier() const
    {
        return SectorData.Identifier;
    }

	inline const FFlareSectorDescription* GetDescription() const
	{
		return SectorDescription;
	}

	inline FFlareSectorOrbitParameters* GetOrbitParameters()
	{
		return &SectorOrbitParameters;
	}

	FString GetSectorName() const;

	FString GetSectorCode() const;

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorStations()
    {
        return SectorStations;
    }

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorShips()
    {
        return SectorShips;
    }

};
