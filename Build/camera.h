#include "gameobject.h"
#include "CameraComponent.h"
#include "level.h"

class Camera :public GameObject
{
public:
	Camera(Level* level);
	~Camera();


	// GameObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void SetLookObject(GameObject* object);

private:
	CameraComponent* cameraComponent;

};
