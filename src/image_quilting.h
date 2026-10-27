#ifndef IMAGE_QUILTING_H
#define IMAGE_QUILTING_H

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include <vector>

namespace godot {

class ImageQuilting : public RefCounted {
    GDCLASS(ImageQuilting, RefCounted)

protected:
    static void _bind_methods();

private:
    static double pixel_error(const Color &a, const Color &b);
    static std::vector<int> find_vertical_seam(
        const Ref<Image> &output, const Ref<Image> &patch,
        int dst_x, int dst_y, int patch_x, int patch_y, int overlap);
    static std::vector<int> find_horizontal_seam(
        const Ref<Image> &output, const Ref<Image> &patch,
        int dst_x, int dst_y, int patch_x, int patch_y, int overlap);

public:
    // Returns an Image of the requested size. Use tolerance=0.1 for the
    // classic "within 10% of the minimum error" candidate-selection rule.
    Ref<Image> synthesize(
        const Ref<Image> &source,
        int output_width,
        int output_height,
        int patch_size = 32,
        int overlap = 8,
        double tolerance = 0.1,
        int seed = 12345
    );
};

} // namespace godot

#endif
