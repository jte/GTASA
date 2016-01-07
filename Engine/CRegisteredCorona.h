#pragma once

class CRegisteredCorona
{
public:
    void Update();
private:
    CVector     Coordinates;            // Where is it exactly.
    DWORD       Identifier;             // Should be unique for each corona. Address or something (0 = empty)
    RwTexture   * pTex;                 // Pointer to the actual texture to be rendered
    FLOAT       Size;                   // How big is this fellow
    FLOAT       NormalAngle;            // Is corona normal (if relevant) facing the camera?
    FLOAT       Range;                  // How far away is this guy still visible
    FLOAT       PullTowardsCam;         // How far away is the z value pulled towards camera.
    FLOAT       HeightAboveGround;      // Stired so that we don't have to do a ProcessVerticalLine every frame
                                        // The following fields are used for trails behind coronas (glowy lights)
    FLOAT       FadeSpeed;              // The speed the corona fades in and out ##SA##
    BYTE        Red, Green, Blue;       // Rendering colour.
    BYTE        Intensity;              // 255 = full
    BYTE        FadedIntensity;         // Intensity that lags behind the given intenisty and fades out if the LOS is blocked
    BYTE        RegisteredThisFrame;    // Has this guy been registered by game code this frame
    BYTE        FlareType;              // What type of flare to render
    BYTE        ReflectionType;         // What type of reflection during wet weather
    BYTE        LOSCheck : 1;           // Do we check the LOS or do we render at the right Z value
    BYTE        OffScreen : 1;          // Set by the rendering code to be used by the update code
    BYTE        JustCreated;            // If this guy has been created this frame we won't delete it (It hasn't had the time to get its OffScreen cleared) ##SA removed from packed byte ##
    BYTE        NeonFade : 1;           // Does the guy fade out when closer to cam
    BYTE        OnlyFromBelow : 1;      // This corona is only visible if the camera is below it. ##SA##
    BYTE        bHasValidHeightAboveGround : 1;
    BYTE        WhiteCore : 1;          // This corona rendered with a small white core.
    BYTE        bIsAttachedToEntity : 1;
    
    CEntity* pEntityAttachedTo;
};