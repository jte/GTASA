#pragma once

class CPlayerInfo
{
public:

private:
    class CPlayerPedSAInterface *pPed;          // Pointer to the player ped (should always be set)
    CPlayerPedDataSAInterface PlayerPedData;    // instance of player variables
    CVehicleSAInterface *pRemoteVehicle;// Pointer to vehicle player is driving remotely at the moment.(NULL if on foot)
    CVehicleSAInterface *pSpecCar;      // which car is using the special collision model
    long        Score;          // Points for this player
    long        DisplayScore;   // Points as they would be displayed
    long        CollectablesPickedUp;   // How many bags of sugar do we have
    long        TotalNumCollectables;   // How many bags of sugar are there to be had in the game

    DWORD       nLastBumpPlayerCarTimer;            // Keeps track of when the last ped bumped into the player car

    DWORD       TaxiTimer;          // Keeps track of how long the player has been in a taxi with a passenger (in msecs)
    DWORD       vehicle_time_counter;  // keeps track of how long player has been in car for driving skill
    bool        bTaxiTimerScore;    // If TRUE then add 1 to score for each second that the player is driving a taxi 
    bool        m_bTryingToExitCar; // if player holds exit car button, want to trigger getout once car slowed enough
                        //  with a passenger

    CVehicle *pLastTargetVehicle;//Last vehicle player tried to enter.

    uint8_t        PlayerState;        // What's going on.

    bool        bAfterRemoteVehicleExplosion;
    bool        bCreateRemoteVehicleExplosion;
    bool        bFadeAfterRemoteVehicleExplosion;
    DWORD       TimeOfRemoteVehicleExplosion;

    DWORD       LastTimeEnergyLost; // To make numbers flash on the HUD
    DWORD       LastTimeArmourLost;

    DWORD       LastTimeBigGunFired;    // Tank guns etc
    DWORD       TimesUpsideDownInARow;  // Make car blow up if car upside down
    DWORD       TimesStuckInARow;       // Make car blow up if player cannot get out.
    
    // working counters to calculate how long player managed stuff
    DWORD       nCarTwoWheelCounter;        // how long has player's car been on two wheels
    FLOAT       fCarTwoWheelDist;
    DWORD       nCarLess3WheelCounter;  // how long has player's car been on less than 3 wheels
    DWORD       nBikeRearWheelCounter;  // how long has player's bike been on rear wheel only
    FLOAT       fBikeRearWheelDist;
    DWORD       nBikeFrontWheelCounter; // how long has player's bike been on front wheel only
    FLOAT       fBikeFrontWheelDist;
    DWORD       nTempBufferCounter;     // so wheels can leave the ground for a few frames without stopping above counters
    // best values for the script to check - will be zero most of the time, only value
    // when finished trick - script should retreve value then reset to zero
    uint32_t       nBestCarTwoWheelsTimeMs;
    float       fBestCarTwoWheelsDistM;
    uint32_t       nBestBikeWheelieTimeMs;
    float       fBestBikeWheelieDistM;
    uint32_t       nBestBikeStoppieTimeMs;
    float       fBestBikeStoppieDistM;

    uint16_t        CarDensityForCurrentZone;
    float       RoadDensityAroundPlayer;    // 1.0f for an average city.

    uint32_t       TimeOfLastCarExplosionCaused;
    long        ExplosionMultiplier;
    long        HavocCaused;            // A counter going up when the player does bad stuff.
    short       TimeLastEaten;          // A counter that starts at 5000, increases by 1 every minute

    float       CurrentChaseValue;      // How 'ill' is the chase at the moment
    
    // 'Special' abilities that gets awarded during the game
    bool        DoesNotGetTired;
    bool        FastReload;
    bool        FireProof;
    uint8_t        MaxHealth;
    uint8_t        MaxArmour;

    // Get-out-of-jail-free & Free-medical-care cards
    bool        bGetOutOfJailFree;      // Player doesn't lose money/weapons next time arrested
    bool        bFreeHealthCare;        // Player doesn't lose money nexed time patched up at hospital

    bool        bCanDoDriveBy;
    
    uint8_t        m_nBustedAudioStatus;
    uint16_t        m_nLastBustMessageNumber;

    CPlayerCrossHair  CrossHair;

    char        m_skinName[32];
    RwTexture*  m_pSkinTexture;

    bool        m_bParachuteReferenced;
    uint32_t       m_nRequireParachuteTimer;
};