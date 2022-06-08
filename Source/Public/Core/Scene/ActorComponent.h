#pragma once

#include <Core/Object.h>
#include <Memory/String.h>

class ENGINE_API ActorComponent : public Object {

public:

    ActorComponent() = default;
 
    /*!
     *  \brief Start is called when the scene has finished loading and constructing all components.
     *  It is called from the parent Actor.
     */
    virtual void start() = 0;

    /*!
     *  \brief Update is called on every engine tick which is NOT tied to rendering thread (and therefore not tied to the frame rate).
     *  It is called from the parent Actor.
     *
     *  \param[in] deltaTime The engine tick delta time
     */
    virtual void update(float deltaTime) = 0;
 
    /*!
     *  \brief Returns the Actor this Component is attached to.
     *
     *  \return The parent actor
     */
    FORCEINLINE class Actor* getParentActor() const { return this->parentActor; }

    /*!
     *  \brief Returns the name of the Component.
     *
     *  \return The name of the Component
     */
    FORCEINLINE String getName() const { return this->name; }

protected:

    /*!
     *  \brief Sets the parent Actor this Component is attached to.
     *
     *  \param[in] parent The parent actor
     */
    void setParentActor(Actor* parent) { this->parentActor = parent; }

private:

    friend class Actor;
 
    /*!
     *  \brief The Actor this SceneComponent is attached to.
     */
    Actor* parentActor = nullptr;

    /*!
     *  \brief Whether this component was registered by the scene.
     */
    bool registered = false;

    /*!
     *  \brief The name of the component.
     */
    String name = "";

};
