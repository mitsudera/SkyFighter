#pragma once
#include "gameobject.h"
class SkySphire :public GameObject
{
public:
	SkySphire(Level* level);
	~SkySphire();


	// GameObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

private:
	MeshComponent* meshComponent;


};

