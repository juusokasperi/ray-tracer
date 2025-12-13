SRCS = main.c							\
	utils/ray_utils.c					\
	color/calculate_color.c				\
	color/checkerboard.c				\
	intersect/cylinder.c				\
	intersect/cone.c					\
	intersect/cone_2.c					\
	intersect/intersect.c				\
	shadows/diffuse.c					\
	shadows/shadow_check.c				\
	shadows/shadow_check_2.c			\
	shadows/calculate_normals.c			\
	hooks/camera_rotation.c				\
	hooks/camera_movement.c				\
	hooks/resize.c						\
	debug/bvh_debug.c					\
	debug/debug.c						\
	rgb_utils/rgb.c						\
	rgb_utils/rgb_2.c					\
	bvh/build_bvh.c						\
	bvh/calculate_aabb.c				\
	bvh/aabb_union.c					\
	bvh/helpers_first_count.c			\
	bvh/helpers_left_right.c			\
	bvh/helpers_build.c					\
	bvh/ray_aabb.c						\
	bvh/ray_aabb_utils.c				\
	bvh/destroy_bvh.c					\
	parser/parser.c						\
	parser/parser_file_name.c			\
	parser/parse_plane.c				\
	parser/parse_cylinder.c				\
	parser/parse_cone.c					\
	parser/parse_light.c				\
	parser/parse_ambient.c				\
	parser/parse_sphere.c				\
	parser/parse_camera.c				\
	parser/parse_rgb.c					\
	parser/parse_dual_rgb.c				\
	parser/parse_float.c				\
	parser/parser_vector.c				\
	parser/parse_identifier.c			\
	parser/parse_object.c				\
	parser/parse_common_utils.c			\
	parser/parse_scene_utils.c			\
	parser/parser_helpers.c				\
	utils/main_utils.c					\
	hooks/camera_controls.c				\
	raycast/raycast_threaded.c			\
	raycast/thread_tile_worker.c		\
	arena/arena_strtrim.c				\
	arena/arena_split_isspace.c			\
	arena/arena_strdup.c

SRCS_DIR =		srcs/
OBJS_DIR =		objs/

OBJS =			$(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

DEPS =			$(OBJS:.o=.d)

NAME =			miniRT

HDRS_PATH =	-I./incs -I$(LIBMLX_DIR)/include

CC =		cc
CFLAGS +=	-Wall -Wextra -Werror -O3 -march=native -mtune=native \
			-fomit-frame-pointer -DNDEBUG -flto -MMD -MP

LIBMLX_DIR =	./MLX42
LIBMLX =		$(LIBMLX_DIR)/build/libmlx42.a
LIBMLX_GIT =	https://github.com/codam-coding-college/MLX42.git
LIBS =			$(LIBMLX) -ldl -lglfw -pthread -lm

# Terminal colors
GREEN =		\033[0;32m
RED =		\033[0;31m
ENDCOLOR =	\033[0m

all:	$(NAME)

$(NAME): $(LIBMLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HDRS_PATH) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME): Compilation complete!$(ENDCOLOR)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(HDRS_PATH)

-include $(DEPS)

$(LIBMLX):
	@if [ ! -d "$(LIBMLX_DIR)" ] || [ -z "$$(ls -A $(LIBMLX_DIR) 2>/dev/null)" ]; then \
		git clone $(LIBMLX_GIT) $(LIBMLX_DIR); \
	fi
	@cmake $(LIBMLX_DIR) -B $(LIBMLX_DIR)/build && make -C $(LIBMLX_DIR)/build -j4
	@echo "$(GREEN)✅ MLX42: Library compiled.$(ENDCOLOR)"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)✅ $(NAME): Clean complete.$(ENDCOLOR)"

fclean:
	@rm -rf $(OBJS_DIR) $(NAME) $(LIBMLX_DIR)
	@echo "$(RED)✅ $(NAME): FClean complete.$(ENDCOLOR)"

re: fclean all

.PHONY: re fclean clean all
