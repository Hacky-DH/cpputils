set(NAME block_queue)

set(sources main.cpp ${NAME}.h)

add_executable(${NAME} ${sources})

target_compile_options(${NAME} PUBLIC -std=c++11 -Wall -Wextra -Werror -g)

target_link_libraries(${NAME} PUBLIC
pthread
)
