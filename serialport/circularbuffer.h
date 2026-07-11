#pragma once
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
/**
 * @brief 通用环形缓冲区（非线程安全）
 * 
 * @tparam T 存储的元素类型
 * @tparam OverwritePolicy 当缓冲区满时，true=覆盖最旧数据，false=丢弃新数据
 */
template <typename T, bool OverwritePolicy = true>
class CircularBuffer {
public:
    // 构造函数，显式指定容量
    explicit CircularBuffer(size_t capacity) 
        : m_capacity(capacity), m_buffer(capacity), m_head(0), m_tail(0), m_count(0) {
        if (capacity == 0) {
            throw std::invalid_argument("CircularBuffer capacity must be > 0");
        }
    }

    // 禁用左值拷贝构造（可根据需要改为深拷贝），允许移动
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete; //禁用左值赋值构造
    CircularBuffer(CircularBuffer&&) = default;
    CircularBuffer& operator=(CircularBuffer&&) = default;

    // ---------- 写入操作 ----------
    
    // 向缓冲区尾部添加元素（根据策略决定是否覆盖）
    void push(const T& value) {
        // constexpr 用于声明可以在编译时求得值的变量或函数,但不是C++11标准
        if (OverwritePolicy) {
            // 覆盖模式：永远不拒绝数据
            m_buffer[m_tail] = value;
            m_tail = (m_tail + 1) % m_capacity;
            
            if (isFull()) {
                // 如果已满，头指针也前进，丢弃最旧数据
                m_head = (m_head + 1) % m_capacity;
            } else {
                ++m_count;
            }
        } else {
            // 丢弃模式：仅在未满时写入
            if (!isFull()) {
                m_buffer[m_tail] = value;
                m_tail = (m_tail + 1) % m_capacity;
                ++m_count;
            }
        }
    }

    // 右值引用版本，支持移动语义（提升性能）
    void push(T&& value) {
        if (OverwritePolicy) {
            m_buffer[m_tail] = std::move(value);
            m_tail = (m_tail + 1) % m_capacity;
            if (isFull()) {
                m_head = (m_head + 1) % m_capacity;
            } else {
                ++m_count;
            }
        } else {
            if (!isFull()) {
                m_buffer[m_tail] = std::move(value);
                m_tail = (m_tail + 1) % m_capacity;
                ++m_count;
            }
        }
    }

    // 尝试写入（仅在非满时成功），返回是否写入成功
    bool tryPush(const T& value) {
        if (isFull()) return false;
        m_buffer[m_tail] = value;
        m_tail = (m_tail + 1) % m_capacity;
        ++m_count;
        return true;
    }

    // ---------- 读取操作 ----------
    
    // 弹出并返回头部元素（若为空则抛出异常）
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("CircularBuffer::pop() on empty buffer");
        }
        T value = std::move(m_buffer[m_head]);
        m_head = (m_head + 1) % m_capacity;
        --m_count;
        return value;
    }

    // 尝试弹出，成功返回 true，失败（空）返回 false
    bool tryPop(T& out_value) {
        if (isEmpty()) return false;
        out_value = std::move(m_buffer[m_head]);
        m_head = (m_head + 1) % m_capacity;
        --m_count;
        return true;
    }

    //读取出相对头位置偏移一定距离的数据
    // 返回相对头指针偏移 offset 的元素的引用（非 const）
    T& at(size_t offset) {
        if (offset >= m_count) {
            throw std::out_of_range("CircularBuffer::at: offset out of range");
        }
        size_t index = (m_head + offset) % m_capacity;
        return m_buffer[index];
    }

    // 返回相对头指针偏移 offset 的元素的常量引用（const 版本）
    const T& at(size_t offset) const {
        if (offset >= m_count) {
            throw std::out_of_range("CircularBuffer::at: offset out of range");
        }
        size_t index = (m_head + offset) % m_capacity;
        return m_buffer[index];
    }

    // ---------- 窥视操作（不移除元素） ----------
    
    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("CircularBuffer::front() on empty buffer");
        }
        return m_buffer[m_head];
    }

    const T& back() const {
        if (isEmpty()) {
            throw std::runtime_error("CircularBuffer::back() on empty buffer");
        }
        // 注意：tail 指向的是下一个待写入位置，所以最后一个有效元素在 tail 的前一个
        size_t last_index = (m_tail == 0) ? (m_capacity - 1) : (m_tail - 1);
        return m_buffer[last_index];
    }

    // ---------- 状态查询 ----------
    
    bool isEmpty() const noexcept { return m_count == 0; }
    bool isFull() const noexcept { return m_count == m_capacity; }
    size_t size() const noexcept { return m_count; }
    size_t capacity() const noexcept { return m_capacity; }
    
    // 清空缓冲区（逻辑清空，不释放内存）
    void clear() noexcept {
        m_head = 0;
        m_tail = 0;
        m_count = 0;
    }

    // 返回当前可用空间（剩余可写入数量）
    size_t available() const noexcept { return m_capacity - m_count; }

private:
    size_t m_capacity;
    std::vector<T> m_buffer;
    size_t m_head;      // 指向读取位置（最旧数据）
    size_t m_tail;      // 指向写入位置（最新数据之后）
    size_t m_count;     // 当前有效元素个数
};