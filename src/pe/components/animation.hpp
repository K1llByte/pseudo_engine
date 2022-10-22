#pragma once

#include <vector>
#include <initializer_list>
		

struct SpriteSlice {
    float x, y;
    float width, height;
};
struct Animation {
    uint16_t slice_idx;
    float duration;
};
struct AnimationSlice {
    uint16_t idx, size;
};
struct SpriteAnimator {
    std::vector<SpriteSlice>    sprite_slices;
    std::vector<Animation>      animations;
    std::vector<AnimationSlice> animation_slices;
    size_t                      active_anim = 0;
    size_t                      active_frame = 0;
    float                       animation_timer = 0.f;
    bool                        flip_h = false;
    bool                        flip_v = false;

    SpriteAnimator(
        std::initializer_list<SpriteSlice>    _sprite_slices,
        size_t _active_anim = 0)
        : sprite_slices(_sprite_slices) {}
    
    // Instead of providing the slices, give number of columns and lines
    SpriteAnimator(
        float width, float height, size_t columns, size_t lines,
        size_t _active_anim = 0)
        : active_anim(_active_anim)
    {
        sprite_slices.reserve(lines*columns);
        const float slice_width = width / columns;
        const float slice_height = height / lines;
        for(size_t i = 0; i < lines; ++i) {
            for(size_t j = 0; j < columns; ++j) {
                sprite_slices.emplace_back(
                    slice_width*j,
                    slice_height*i,
                    slice_width,
                    slice_height
                );
            }
        }
    }

    SpriteAnimator& set_active(size_t idx) {
        // NDEBUG Check if out of bounds 
        if(active_anim != idx) {
            active_anim = idx;
            active_frame = 0;
            animation_timer = animations[0].duration;
        }
        return *this;
    }

    SpriteAnimator& flip_horizontal(bool flip) {
        flip_h = flip;
        return *this;
    }

    SpriteAnimator& flip_vertical(bool flip) {
        flip_v = flip;
        return *this;
    }

    SpriteAnimator& add_animation(const std::vector<Animation>& animation) {
        // NDEBUG: Check if size > 0
        assert(animation.size() > 0 && "Can't have 0 frame animations");
        animation_slices.emplace_back(animations.size(), animation.size());
        animations.insert(animations.end(), animation.begin(), animation.end());
        return *this;
    }

    SpriteSlice get_active_sprite() {
        // Get sprite slice
        auto slice = sprite_slices[animations[active_frame].slice_idx];
        if(flip_h) {
            // Flip horizontally
            slice.x += slice.width;
            slice.width *= -1;
        }
        if(flip_v) {
            // Flip vertically
            slice.y += slice.height;
            slice.height *= -1;
        }
        return slice;
    }

    void update(float dt) {
        animation_timer -= dt;
        if(animation_timer < 0.f) {
            const auto anim_slice = animation_slices[active_anim];
            // Set new active_frame
            active_frame = (active_frame-anim_slice.idx+1) % anim_slice.size + anim_slice.idx;
            // Set new animation_timer
            animation_timer = animations[active_frame].duration;
        }
    }
};