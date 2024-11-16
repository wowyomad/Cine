#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

enum class Direction
{
    None = 0,
    Right,
    Left,
    Up,
    Down
};

struct DirectionTiming
{
    float Right = 0.0f, Left = 0.0f, Up = 0.0f, Down = 0.0f;

    Direction GetLatestDirection() const
    {
        if (Right > Left && Right > Up && Right > Down)
        {
            return Direction::Right;
        }
        if (Left > Right && Left > Up && Left > Down)
        {
            return Direction::Left;
        }
        if (Up > Left && Up > Right && Up > Down)
        {
            return Direction::Up;
        }
        if (Down > Left && Down > Right && Down > Up)
        {
            return Direction::Down;
        }

        return Direction::None;
    }

    SERIALIZE_CLASS(DirectionTiming,
                    FIELD(Right);
                    FIELD(Left);
                    FIELD(Up);
                    FIELD(Down);)
};

class PlayerController : public NativeScript
{
public:
    void OnCreate() override
    {
    }

    void OnUpdate(Timestep ts) override
    {
        UpdateDirectionTimings(ts);
        HandleMouseInput(ts);
        HandleKeyInput(ts);
    }

    void HandleKeyInput(float ts)
    {

        glm::vec3 moveDirection = GetMoveDirection(m_MovementDirectionTiming);

        if (glm::length(moveDirection) > 0.0f)
        {
            moveDirection = glm::normalize(moveDirection);
            LocalTranslation() += moveDirection * (ts * m_MoveSpeed);
        }

        if (!UseMouse)
        {
            glm::vec3 lookDirection = GetLookDirection(m_LookDirectionTiming, moveDirection);
            SetLookRotation(lookDirection);
        }
    }

    void UpdateDirectionTimings(float ts)
    {
        m_Timer += ts;
        if (Input::IsKeyDown(Key::D))
            m_MovementDirectionTiming.Right = m_Timer;
        if (Input::IsKeyDown(Key::A))
            m_MovementDirectionTiming.Left = m_Timer;
        if (Input::IsKeyDown(Key::W))
            m_MovementDirectionTiming.Up = m_Timer;
        if (Input::IsKeyDown(Key::S))
            m_MovementDirectionTiming.Down = m_Timer;

        if (Input::IsKeyUp(Key::D))
            m_MovementDirectionTiming.Right = 0.0f;
        if (Input::IsKeyUp(Key::A))
            m_MovementDirectionTiming.Left = 0.0f;
        if (Input::IsKeyUp(Key::W))
            m_MovementDirectionTiming.Up = 0.0f;
        if (Input::IsKeyUp(Key::S))
            m_MovementDirectionTiming.Down = 0.0f;

        if (Input::IsKeyDown(Key::Right))
            m_LookDirectionTiming.Right = m_Timer;
        if (Input::IsKeyDown(Key::Left))
            m_LookDirectionTiming.Left = m_Timer;
        if (Input::IsKeyDown(Key::Up))
            m_LookDirectionTiming.Up = m_Timer;
        if (Input::IsKeyDown(Key::Down))
            m_LookDirectionTiming.Down = m_Timer;

        if (Input::IsKeyUp(Key::Right))
            m_LookDirectionTiming.Right = 0.0f;
        if (Input::IsKeyUp(Key::Left))
            m_LookDirectionTiming.Left = 0.0f;
        if (Input::IsKeyUp(Key::Up))
            m_LookDirectionTiming.Up = 0.0f;
        if (Input::IsKeyUp(Key::Down))
            m_LookDirectionTiming.Down = 0.0f;
    }

    glm::vec3 GetMoveDirection(const DirectionTiming &directionTiming) const
    {
        glm::vec3 moveDirection = {};
        switch (directionTiming.GetLatestDirection())
        {
        case Direction::Left:
            moveDirection.x = -1.0f;
            break;
        case Direction::Right:
            moveDirection.x = +1.0f;
            break;
        case Direction::Up:
            moveDirection.y = +1.0f;
            break;
        case Direction::Down:
            moveDirection.y = -1.0f;
            break;
        }
        return moveDirection;
    }

    glm::vec3 GetLookDirection(const DirectionTiming &directionTiming, const glm::vec3 &moveDirection) const
    {
        glm::vec3 lookDirection = {};
        switch (directionTiming.GetLatestDirection())
        {
        case Direction::Left:
            lookDirection.x = -1.0f;
            break;
        case Direction::Right:
            lookDirection.x = +1.0f;
            break;
        case Direction::Up:
            lookDirection.y = +1.0f;
            break;
        case Direction::Down:
            lookDirection.y = -1.0f;
            break;
        }

        if (glm::length(lookDirection) > 0.0f)
        {
            return glm::normalize(lookDirection);
        }
        else if (glm::length(moveDirection) > 0.0f)
        {
            return moveDirection;
        }
        else
        {
            return m_LastLookDirection;
        }
    }

    void SetLookRotation(const glm::vec3 &lookDirection)
    {
        float angle = atan2(lookDirection.y, lookDirection.x);
        LocalRotation().z = angle;
        m_LastLookDirection = lookDirection;
    }

    void HandleMouseInput(float ts)
    {
        if (!UseMouse)
            return;

        glm::vec2 mousePosition = Input::GetMousePosition();

        auto &transform = Transform();
        glm::vec3 mouseWorldPosition = Input::ToWorldSpace(mousePosition);
        mouseWorldPosition.z = LocalTranslation().z;

        glm::vec3 direction = glm::normalize(mouseWorldPosition - transform.Translation);
        float angle = atan2(direction.y, direction.x);

        transform.Rotation.z = angle;

        if (Input::IsMouseButtonDown(Mouse::ButtonLeft))
        {
            OnMouseButtonClicked(mousePosition);
        }
    }

    void OnMouseButtonClicked(glm::vec2 screenPosition)
    {
        auto world = Input::ToWorldSpace(screenPosition);
        auto screen = Input::ToScreenSpace(world);
    }

private:
    float m_MoveSpeed = 5.0f;

    bool UseMouse = false;
    float m_Timer = 0.0f;

    glm::vec3 m_LastLookDirection = {0.0f, -1.0f, 0.0f};
    glm::vec3 m_LastMoveDirection = {0.0f, 0.0f, 0.0f};

    DirectionTiming m_MovementDirectionTiming;
    DirectionTiming m_LookDirectionTiming;

    SERIALIZE_CLASS(PlayerController,
                    FIELD(m_MoveSpeed);
                    FIELD(UseMouse);
                    FIELD(m_LastLookDirection);
                    FIELD(m_MovementDirectionTiming);
                    FIELD(m_LookDirectionTiming))
                    
public:
    PlayerController &operator=(const PlayerController &other)
    {
        if (this == &other)
            return *this;

        m_MoveSpeed = other.m_MoveSpeed;
        UseMouse = other.UseMouse;
        m_LastLookDirection = other.m_LastLookDirection;
        m_MovementDirectionTiming = other.m_MovementDirectionTiming;
        m_LookDirectionTiming = other.m_LookDirectionTiming;

        NativeScript::operator=(other);
        return *this;
    }
};
