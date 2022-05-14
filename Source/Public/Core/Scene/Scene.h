#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Scene/CameraComponent.h>
#include <Scene/CameraManager.h>
#include <Console/Console.h>
#include <Memory/Array.h>
#include <Memory/UniquePointer.h>
#include <Logging/Logger.h>

struct Color {

    static void setByte(UInt32& bytes, UInt8 byte, int pos) {
        bytes &= ~(static_cast<UInt32>(0xff) << (4 * pos));
        bytes |= (static_cast<UInt32>(byte) << (4 * pos));
    }

    Color(const UInt8 red, const UInt8 green, const UInt8 blue, const UInt8 alpha){
        
        this->value = 0;
        setByte(this->value, red, 0);
        setByte(this->value, green, 0);
        setByte(this->value, blue, 0);
        setByte(this->value, alpha, 0);
        
    }
    
    UInt32 value;

};

/*!
 * A scene is an object that represents the world. It holds SceneComponents and SceneObjects, and manages their lifetimes.
 */
class ENGINE_API Scene : public Object {

public:

    enum class State
    {
        Unloaded,
        Loading,
        Loaded,
        Unloading
    };

    /*!
     * The scene constructor. Initializes the private variables.
     */
    Scene();

    /*!
    * Loads the scene. It sets up and constructs every object in the scene.
    */
    void load();
    
    virtual void start() = 0;

    /*!
    * Called when the scene is destroyed. It destroys every object in the scene.
    */
    virtual void cleanup();

    void listObjects() const;
    
    void listComponents() const;

    NODISCARD FORCEINLINE State getState() const { return this->state; }
    
    NODISCARD FORCEINLINE virtual String getSceneName() const = 0;

    void setWorldColor(UInt32 color) const;

    NODISCARD SceneObject* getObjectByIndex(size_t index) const;
    
    /*!
     *  Instantiates a new SceneObject in the scene
     *
     *  \tparam T The SceneObject class to instantiate
     *  \tparam Args A variadic template type
     *  \param[in] transform The world Transform of the SceneObject
     *  \param[in] args The arguments to pass to the SceneObject's constructor
     *  \return The newly created SceneObject
     *
     */
    template<class T, typename ... Args>
    WeakPointer<T> createObject(Transform const &transform, Args &&... args) {
        
        // Check if the class we are trying to instantiate is a SceneObject.
        static_assert(std::is_base_of_v<SceneObject, T>, "T should inherit from SceneObject");

        // Instantiate the SceneObject
        SharedPointer<T> object = SharedPointer<T>::make(std::forward<Args>(args)...);
        
        // Create the components
        object->createComponents(this, transform);

        // If the SceneObject did not setup a root component, destroy it.
        if (!object->rootComponent) {
            
            Console::getLogger()->debug("Destroying object of class {} as it did not setup a root component at construct time.", object->getClassName());
            
            return nullptr;

        }
        
        // Setup the input on the created SceneObject
        setupInput(object.raw());

        // Register the SceneObject
        registeredObjects += object;
        
        return object.weak();

    }

    /*!
     *  Instantiates a new SceneComponent in the scene
     *
     *  \tparam T The SceneComponent class to instantiate
     *  \tparam Args A variadic template type
     *  \param[in] relativeTransform The relative Transform of the SceneComponent
     *  \param[in] args The arguments to pass to the SceneComponent's constructor
     *  \return The newly created SceneComponent
     *
     */
    template<class T, typename ... Args>
    WeakPointer<T> createComponent(Transform const &relativeTransform, Args &&... args) {

        // Check if the class we are trying to instantiate is a SceneComponent.
        static_assert(std::is_base_of_v<SceneComponent, T>, "T should inherit from SceneComponent");
        
        // Instantiate the SceneComponent
        SharedPointer<T> component = SharedPointer<T>::make(std::forward<Args>(args)...);
        
        component->scene = this;
        
        component->construct(relativeTransform);
        
        // Mark it as registered (bad, will probably be changed later)
        component->registered = true;

        // If the created component is a CameraComponent, it is passed to the camera manager.
        if (component->template inherits<CameraComponent>()) {

            auto a = component.weak();
            cameraManager->registerCamera(*dynamic_cast<WeakPointer<CameraComponent>*>(&a));

        }

        // Register the SceneComponent
        registeredComponents += component;
        
        return component.weak();

    }

    /*!
    * Destroys a SceneObject from the Scene.
    *
    *  \param[in] object The object to destroy
    *  \return Whether the object was destroyed
    */
    virtual bool destroyObject(SceneObject *object);

    /*!
     * Destroys a SceneComponent from the Scene.
     *
     *  \param[in] component The component to destroy
     *  \return Whether the component was destroyed
     */
    virtual bool destroyComponent(SceneComponent *component);

    /*!
     * Gets this Scene's CameraManager.
     *
     *  \return The Scene CameraManager.
     */
    FORCEINLINE CameraManager *getCameraManager() const {
        return this->cameraManager.raw();
    }

    template<typename T>
    WeakPointer<T> getFirstComponentOfClass() const {

        for (auto& component : registeredComponents) {

            if(component->inherits<T>()) {
                
                return component.weak();
                
            }
                        
        }

        return WeakPointer<T>();
        
    }

protected:

    friend class Engine;

    /*!
     * Renders all renderable components in the scene. This function is called on the render thread.
     */
    virtual void renderComponents() const;

    /*!
     *  Updates every object on the scene.
     *
     *  \param[in] deltaTime The engine delta time
     */
    virtual void update(float deltaTime);

    /*!
     * Allows a newly created SceneObject to bind input events.
     *
     *  \param[in] object The target SceneObject
     */
    virtual void setupInput(SceneObject *object);

    /*!
     * An automatically managed array storing references to all SceneObjects in the scene.
     * The SceneObjects are automatically released when the scene is destroyed.
     */
    Array<SharedPointer<SceneObject>> registeredObjects = Array<SharedPointer<SceneObject>>(50);

    /*!
     * An automatically managed array storing references to all SceneComponents in the scene.
     * The SceneComponents are automatically released when the scene is destroyed.
     */
    Array<SharedPointer<SceneComponent>> registeredComponents = Array<SharedPointer<SceneComponent>>(50);

    /*!
     * The scene's CameraManager.
     */
    UniquePointer<CameraManager> cameraManager;
    
    State state = State::Unloaded;

};
