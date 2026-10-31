#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, UP, RIGHT, DOWN }; // For walking
enum EntityStatus { ACTIVE, INACTIVE };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::vector<int>                      mAnimationAtlas;
    std::vector<int>                      mAnimationIndices;
    Direction                             mDirection;
    int                                   mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    bool  mLandable    = false;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingBottom = false;
    bool mIsCollidingSides  = false;

    EntityStatus mEntityStatus = ACTIVE;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void resetColliderFlags()
    {
        mIsCollidingBottom = false;
        mIsCollidingSides  = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::vector<int>                      animationAtlas
    );
    ~Entity();

    void update(float deltaTime, Entity *collidableEntities, int collisionCheckCount);
    void render();

    void accelerateLeft() { mAcceleration.x = -50.0f; }
    void accelerateRight() { mAcceleration.x = 50.0f; }
    void stopAccelerating() { mAcceleration.x = 0.0f; }
    bool isCollidingBottom(Entity *other) const;
    bool isCollidingX(Entity*) const;
    bool isCollidingY(Entity*) const;

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }

    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isLandable() const { return mLandable; }

    std::vector<int> getAnimationAtlas() const { return mAnimationAtlas; }
    void displayCollider(Color);

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions)
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed = newSpeed;                       }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setAngle(float newAngle)
        { mAngle = newAngle;                       }
    void setLandable(bool landable) { mLandable = landable; }
    void resetVelocity() { mVelocity = {0.0f, 0.0f}; }
};

#endif // ENTITY_H