
# BTX Model Format Specification draft

Models in BTX represent a lot of in-game stuff, so there are a few different model types:
- Static models
    - Used for props, level geometry etc.
- Skinned models (skeletal animation)
    - Used for character bodies and various other kinds of animating models
- Morphing models (vertex animation)
    - Usable for character heads (lip sync), piles of organic matter

## Model structure

Models may contain any of the following:
- Meshes, represented by:
    - Vertex data:
        - Position
        - Normal and tangent (optional)
        - UV map(s) (optional)
        - Colour(s) (optional)
        - 4 bone weights (optional)
    - Surfaces:
        - Material name
        - Vertex indices
    - A parent bone (optional)
- Bones:
    - Name
    - Transformation (position & orientation)
    - Child bone
- Bone animations:
    - Name
    - Framerate
    - Animation events: (optional)
        - Frame at which it occurs (float)
        - Event ID (int)
        - Event parameters (string)
    - Recorded bone transforms (4x4 matrices)
- Attachments: (so game code can attach stuff onto stuff)
    - Name
    - Parent bone
    - Transform relative to parent bone
- Simple animation blends: (to blend between two animations)
    - Name
    - Animation A
    - Animation B
    - Blend factor (0 to 1)
- Two-sided animation blends: (to blend between three animations)
    - Name
    - Animation A
    - Animation B
    - Animation C
    - Blend factor (-1 to +1)
        - If BF is negative, blend between A and B
        - If BF is positive, blend between B and C
- Animation channels: (so that multiple anims can be played on the same model)
    - Name
    - List of affected bones
- Metadata (if you want to embed extra data into the model, like parameters for physics objects)

## Some notes

- All models can be represented by this structure. You can have meshes and not have anything else, which is perfectly fine for static models. You can have a skeleton and nothing else, so other models can refer to said skeleton. Same goes for animations.

- LODs can be implemented as just another mesh. Physics proxies are the same, parented to a bone though.

- Vertex colours can be used for terrain blending, water movement, various vegetation coefficients, and all that is up to the render frontend to decide.

- Having multiple meshes can be essentially used to do vertex animation.

- Game code may additionally control bones at any time. Game code may also extract motion from any given bone. The model specification does not really care about this though.

- It's a text format for the sake of easy editing.

## Syntax
*TODO: write*

## Example model
*TODO: write*
