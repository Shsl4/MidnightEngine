#pragma once

#include <EngineTypes.h>
#include <Math/Vector3.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>
#include <Math/Transform.h>
#include <Scene/Scene.h>
#include <Object.h>

/*!
 * A SceneObject represents an entity that holds SceneComponents.
 */
class SceneObject : public Object {

public:

    /*!
     * The SceneObject constructor. It takes no parameters by default.
     */
    SceneObject() = default;

    /*!
     * Start is called when the scene has finished loading and constructing all components and objects.
     * It is called directly from the scene.
     */
    virtual void start();
    
    /*!
     * Update is called on every engine tick which is NOT tied to rendering thread (and therefore not tied to the frame rate).
     * It is called directly from the scene.
     *
     *  \param[in] deltaTime The engine tick delta time
     */
    virtual void update(float deltaTime);

    /*!
     *  Function called when a SceneComponent is attached on this SceneObject.
     *
     *  \param[in] component The component newly attached
     */
    virtual void onComponentAttached(class SceneComponent *component);

    /*!
     *  Function called when a SceneComponent is detached from this SceneObject.
     *
     *  \param[in] component The component newly detached
     */
    virtual void onComponentDetached(class SceneComponent *component);

    /*!
     * Function called by the scene when the object is created. SceneObjects must override this method to
     * create their SceneComponents.
     *
     * \param[in] scene The scene this object was created in.
     * \param[in] transform The SceneObject world transform.
     */
    virtual void createComponents(class Scene *scene, Transform transform) = 0;

    /*!
     * Returns whether this object is valid.
     *
     * \return Whether this object is valid.
     */
    FORCEINLINE bool isValid() const {
        return true;
    }

    /*!
     * Returns this SceneObject root component
     *
     * \return This object's root component
     */
    FORCEINLINE class SceneComponent *getRootComponent() const {
        return this->rootComponent;
    }

protected:

    /*!
     * Called after the SceneObject is created. It allows input binding.
     *
     *  \param[in] manager The engine InputManager
     */
    virtual void setupInput(class InputManager *manager);

    /*!
     * Sets the input component as this object's root component
     *
     *  \param[in] component The new root component
     */
    void setRootComponent(class SceneComponent *component);

private:

    friend class Scene;

    /*!
     * This object's root component
     */
    class SceneComponent *rootComponent;

};
