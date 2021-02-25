CC = g++
CFLAGS = -ggdb3 -I ../src/ -std=c++17

test_math: test_math.cc ../src/core/math.cc
	$(CC) $(CFLAGS) -o $@ $^

test_ray: test_ray.cc
	$(CC) $(CFLAGS) -o $@ $^

test_cam: test_cam.cc ../src/core/camera.cc
	$(CC) $(CFLAGS) -o $@ $^

test_img: test_img.cc ../src/util/image.cc
	$(CC) $(CFLAGS) -o $@ $^

test_sphere: test_sphere.cc ../src/objects/sphere.cc ../src/core/object.cc
	$(CC) $(CFLAGS) -o $@ $^

test_perlin: test_perlin.cc ../src/util/image.cc
	$(CC) $(CFLAGS) -o $@ $^

test_converter: test_converter.cc ../src/util/map_converter.cc ../src/util/image.cc
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test_math test_ray test_cam test_sphere test_img test*.ppm