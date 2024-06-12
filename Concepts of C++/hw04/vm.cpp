#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {

using item_t = int64_t;

// 定义用于堆栈失败和段错误的异常类
struct vm_stackfail : public std::exception {
    const char* what() const noexcept override { return "VM堆栈失败"; }
};

struct vm_segfault : public std::exception {
    const char* what() const noexcept override { return "VM段错误"; }
};

// 定义VM状态
struct vm_state {
    size_t pc = 0; // 程序代码中的执行位置
    std::stack<item_t> stack; // 执行数据堆栈

    // 注册的指令
    std::unordered_map<std::string, size_t> instruction_ids;
    std::unordered_map<size_t, std::function<void()>> instruction_actions;
};


class vm {
public:
    static std::tuple<item_t, std::string> run(vm_state& vm, const std::vector<std::string>& code);

    static void register_instruction(vm_state& vm, const std::string& instruction, size_t id, std::function<void()> action);

    static void create_vm(vm_state& vm);
};

// 注册带有其ID和操作的指令
void register_instruction(vm_state& vm, const std::string& instruction, size_t id, std::function<void()> action) {
    vm.instruction_ids[instruction] = id;
    vm.instruction_actions[id] = action;
}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }

    return code;
}

// 使用提供的代码运行VM
std::tuple<item_t, std::string> run(vm_state& vm, const std::vector<std::string>& code) {
    while (vm.pc < code.size()) {
        std::istringstream iss(code[vm.pc]);
        std::string instruction;
        iss >> instruction;

        // 根据解析的命令执行指令
        if (vm.instruction_ids.find(instruction) != vm.instruction_ids.end()) {
            size_t id = vm.instruction_ids[instruction];
            if (vm.instruction_actions.find(id) != vm.instruction_actions.end()) {
                try {
                    vm.instruction_actions[id]();
                } catch (const vm_stackfail&) {
                    throw vm_stackfail{};
                }
            } else {
                throw vm_segfault{};
            }
        } else {
            throw vm_segfault{};
        }

        vm.pc++;
    }

    if (!vm.stack.empty()) {
        item_t result = vm.stack.top();
        vm.stack.pop();
        return { result, "" };
    }

    return { 0, "" };
}

// 创建VM并注册指令
void create_vm(vm_state& vm) {
    // 使用其ID和操作注册指令
    register_instruction(vm, "LOAD_CONST", 1, [&vm]() {
        int value;
        std::cin >> value;
        vm.stack.push(value);
    });

    register_instruction(vm, "PRINT", 2, [&vm]() {
        if (!vm.stack.empty()) {
            std::cout << "TOS: " << vm.stack.top() << std::endl;
        }
    });

    // ... 根据您的需求注册其他指令

    register_instruction(vm, "EXIT", 10, [&vm]() {
        if (!vm.stack.empty()) {
            vm.stack.pop();
        }
    });

    // ... 类似地注册其他指令
}




} // namespace vm
