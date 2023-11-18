#version 330 core
    out vec4 FragColor;//给出输出变量
    uniform vec4 ourColor;//给出uniform变量
    void main()
    {
       FragColor=ourColor;
    }