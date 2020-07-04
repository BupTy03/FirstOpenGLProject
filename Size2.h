#pragma once


template<typename T>
struct Size2
{
    constexpr Size2() noexcept : width_{}, height_{} {}
    explicit constexpr Size2(T width, T height) noexcept : width_{width}, height_{height} {}

    constexpr inline T width() const noexcept { return width_; }
    constexpr inline T height() const noexcept { return height_; }

    constexpr inline Size2 withWidth(T width) const noexcept { return Size2{width, height_ }; }
    constexpr inline Size2 withHeight(T height) const noexcept { return Size2{width_, height }; }

    constexpr inline Size2 plusWidth(T width) const noexcept { return Size2{width_ + width, height_ }; }
    constexpr inline Size2 plusHeight(T height) const noexcept { return Size2{width_, height_ + height }; }

    friend constexpr inline bool operator==(const Size2& lhs, const Size2& rhs) noexcept
    {
        return lhs.width() == rhs.width() && lhs.height() == rhs.height();
    }

    friend constexpr inline bool operator!=(const Size2& lhs, const Size2& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend constexpr const Size2 operator*(const Size2& p, T mult) noexcept
    {
        return Size2{
                p.width() * mult,
                p.height() * mult
        };
    }

    friend constexpr const Size2 operator/(const Size2& p, T divider) noexcept
    {
        return Size2{
                p.width() / divider,
                p.height() / divider
        };
    }

    friend constexpr const Size2 operator+(const Size2& lhs, const Size2& rhs) noexcept
    {
        return Size2{
                lhs.width() + rhs.width(),
                lhs.height() + rhs.height()
        };
    }

    friend constexpr const Size2 operator-(const Size2& lhs, const Size2& rhs) noexcept
    {
        return Size2{
                static_cast<T>(lhs.width() - rhs.width()),
                static_cast<T>(lhs.height() - rhs.height())
        };
    }

private:
    T width_;
    T height_;
};
