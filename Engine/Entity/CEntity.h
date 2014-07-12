#pragma once

enum eEntityType
{
    ENTITY_TYPE_NOTHING,
    ENTITY_TYPE_BUILDING,
    ENTITY_TYPE_VEHICLE,
    ENTITY_TYPE_PED,
    ENTITY_TYPE_OBJECT,
    ENTITY_TYPE_DUMMY,
    ENTITY_TYPE_NOTINPOOLS
};

enum eEntityStatus
{
    STATUS_PLAYER,
    STATUS_PLAYER_PLAYBACKFROMBUFFER,
    STATUS_SIMPLE,
    STATUS_PHYSICS,
    STATUS_ABANDONED,
    STATUS_WRECKED,
    STATUS_TRAIN_MOVING,
    STATUS_TRAIN_NOT_MOVING,
    STATUS_HELI,
    STATUS_PLANE,
    STATUS_PLAYER_REMOTE,
    STATUS_PLAYER_DISABLED,
    STATUS_TRAILER,
    STATUS_SIMPLE_TRAILER
};

class CEntity : public CPlaceable
{
public:
    // virtual methods
	virtual void Add(); // DONE
	virtual void Add(const CRect& rect);
    virtual void CreateRwObject();
	virtual void DeleteRwObject();
    virtual void FlagToDestroyWhenNextProcessed();
    virtual CRect GetBoundRect() const;
    virtual void PreRender();
    virtual void ProcessCollision();
	virtual void ProcessControl();
    virtual void ProcessShift();
    virtual void Remove();
    virtual void RemoveLighting(bool bAdjustGlobal);
	virtual void Render();
	virtual void SetIsStatic(bool bIsStatic);
	virtual void SetModelIndex(unsigned int modelIndex); // DONE
	virtual void SetModelIndexNoCreate(unsigned int);
	virtual bool SetupLighting();
	virtual void SpecialEntityCalcCollisionSteps(bool&, bool&);
	virtual void SpecialEntityPreCollisionStuff(class CPhysical*, bool, bool&, bool&, bool&, bool&);
	virtual void Teleport(CVector, unsigned char);
	virtual bool TestCollision(bool);
    //
	void AttachToRwObject(RwObject *pObject, bool bUpdateMatrix);
	void BuildWindSockMatrix();
	CEntity();
	void CalculateBBProjection(CVector*, CVector*, CVector*, CVector*);
	void CleanUpOldReference(CEntity **ppRef);
	void CreateEffects();
	void DestroyEffects();
	void DetachFromRwObject();
	bool DoesNotCollideWithFlyers();
	void FindTriggerPointCoors(int);
	void GetBoundCentre() const;
	void GetBoundCentre(CVector&) const;
	CColModel *GetColModel();
	size_t GetDistanceFromCentreOfMassToBaseOfModel();
	bool GetIsBoundingBoxOnScreen();
	bool GetIsOnScreen();
	bool GetIsTouching(CEntity*) const;
	bool GetIsTouching(CVector const&, float) const;
	//C2dEffect GetRandom2dEffect(int, unsigned char);
	bool HasPreRenderEffects();
	bool IsEntityOccluded();
	bool IsVisible();
    bool LivesInThisNonOverlapSector(int iSectorCenterX, int iSectorCenterY);
	void ModifyMatrixForBannerInWind();
	void ModifyMatrixForCrane();
	void ModifyMatrixForTreeInWind();
	void PreRenderForGlassWindow();
	void ProcessLightsForEntity();
	void PruneReferences();
	void RegisterReference(CEntity**);
	void RemoveEscalatorsForEntity();
	void RenderEffects();
	void ResolveReferences();
	void SetRwObjectAlpha(int);
	void SetupBigBuilding();
	void UpdateAnim();
	void UpdateRpHAnim(); // DONE
	void UpdateRwFrame(); // DONE
	virtual ~CEntity();

    //
    RwObject* GetRwObject() const;
    bool GetIsUnderwater() const;
    bool GetIsBigBuilding() const;
    bool GetIsDrawnLast() const;
    bool GetIsBackfaceCulled() const;
    size_t GetModelIndex() const;
    //
    void SetIsBeingRendered(bool beingRendered);
protected:
private:
    RwObject* m_rwObject; // 24
    /********** BEGIN CFLAGS **************/
    unsigned long m_usesCollision : 1;           // does entity use collision
    unsigned long bCollisionProcessed : 1;  // has object been processed by a ProcessEntityCollision function
    unsigned long bIsStatic : 1;                // is entity static
    unsigned long bHasContacted : 1;            // has entity processed some contact forces
    unsigned long bIsStuck : 1;             // is entity stuck
    unsigned long bIsInSafePosition : 1;        // is entity in a collision free safe position
    unsigned long bWasPostponed : 1;            // was entity control processing postponed
    unsigned long bIsVisible : 1;               //is the entity visible
    
    unsigned long m_isBigBuilding : 1;           // Set if this entity is a big building
    unsigned long bRenderDamaged : 1;           // use damaged LOD models for objects with applicable damage
    unsigned long bStreamingDontDelete : 1; // Dont let the streaming remove this 
    unsigned long bRemoveFromWorld : 1;     // remove this entity next time it should be processed
    unsigned long bHasHitWall : 1;              // has collided with a building (changes subsequent collisions)
    unsigned long m_imBeingRendered : 1;     // don't delete me because I'm being rendered
    unsigned long bDrawLast :1;             // draw object last
    unsigned long bDistanceFade :1;         // Fade entity because it is far away
    
    unsigned long m_dontCastShadowsOn : 1;       // Dont cast shadows on this object
    unsigned long bOffscreen : 1;               // offscreen flag. This can only be trusted when it is set to true
    unsigned long bIsStaticWaitingForCollision : 1; // this is used by script created entities - they are static until the collision is loaded below them
    unsigned long bDontStream : 1;              // tell the streaming not to stream me
    unsigned long bUnderwater : 1;              // this object is underwater change drawing order
    unsigned long bHasPreRenderEffects : 1; // Object has a prerender effects attached to it
    unsigned long bIsTempBuilding : 1;          // whether or not the building is temporary (i.e. can be created and deleted more than once)
    unsigned long m_dontUpdateHierarchy : 1; // Don't update the aniamtion hierarchy this frame
    
    unsigned long bHasRoadsignText : 1;     // entity is roadsign and has some 2deffect text stuff to be rendered
    unsigned long bDisplayedSuperLowLOD : 1;
    unsigned long bIsProcObject : 1;            // set object has been generate by procedural object generator
    unsigned long bBackfaceCulled : 1;          // has backface culling on
    unsigned long bLightObject : 1;         // light object with directional lights
    unsigned long bUnimportantStream : 1;       // set that this object is unimportant, if streaming is having problems
    unsigned long bTunnel : 1;          // Is this model part of a tunnel
    unsigned long bTunnelTransition : 1;        // This model should be rendered from within and outside of the tunnel
    /********** END CFLAGS **************/

    int16_t        m_ucSeedColFlags; //32
    int16_t        m_modelIndex;//34
    class CReference *m_pRef; //36
    
    class CLinkList<CEntity*> *m_pLastRenderedLink;
    
    uint16_t m_nScanCode;           // used to avoid duplicate queries in sector list processing
    uint8_t m_iplIndex;            // used to define which IPL file object is in +46
    uint8_t m_areaCode;            // used to define what objects are visible at this point +47
    
    // LOD shit
    CEntity *m_pLod; // 48
    // num child higher level LODs
    uint8_t numLodChildren; // 52
    // num child higher level LODs that have been rendered
    signed char numLodChildrenRendered; // 53

    //********* BEGIN CEntityInfo **********//
    uint8_t m_type : 3; // what type is the entity              
    uint8_t m_status : 5;               // control status       // 54
    //********* END CEntityInfo **********//

    uint8_t m_pad0; // 55
};