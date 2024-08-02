#pragma once
#include "gameobject.h"
class BlueField :public GameObject
{
public:
	BlueField(Level* level);
	~BlueField();


	// GameObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	virtual void Draw(void) override;

private:
	MeshComponent* mesh;
};

