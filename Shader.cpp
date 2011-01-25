/*
 *  Shader.cpp
 *  CS248-Final-Project
 *
 *  Created by Matthew Fichman on 1/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Shader.h"
#include <fstream>

#define ERROR_BUFSIZE 1024

Shader::Shader(const std::string& path) :
	path_(path),
	vertexShader_(0),
	fragmentShader_(0),
	program_(0),
	loaded_(false) {

	const GLchar* source[1];
    int length = 0;
	
	// Load the fragment shader and compile
	std::vector<char> fragmentSource = readSource(path + ".frag.glsl");
	source[0] = &fragmentSource.front();
    length = fragmentSource.size();
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_, 1, source, &length);
    glCompileShader(fragmentShader_);
		
	// Load the vertex shader and compile
	std::vector<char> vertexSource = readSource(path + ".vert.glsl");
	source[0] = &vertexSource.front();
    length = vertexSource.size();
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_, 1, source, &length);
    glCompileShader(vertexShader_);
	
	// Create the vertex program
	program_ = glCreateProgram();
	glAttachShader(program_, fragmentShader_);
	glAttachShader(program_, vertexShader_);
	glLinkProgram(program_);
	
	// Error checking
	//glGetProgramiv(program_, GL_LINK_STATUS, (GLint*)&loaded_);
    glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, (GLint*)&loaded_);
	if (!loaded_) {
		GLchar tempErrorLog[ERROR_BUFSIZE];
		GLsizei length;
		
		
		glGetShaderInfoLog(fragmentShader_, ERROR_BUFSIZE, &length, tempErrorLog);
        errors_ += "Fragment shader errors:\n";
		errors_ += std::string(tempErrorLog, length) + "\n";
		glGetShaderInfoLog(vertexShader_, ERROR_BUFSIZE, &length, tempErrorLog);
        errors_ += "Vertex shader errors:\n";
		errors_ += std::string(tempErrorLog, length) + "\n";
		glGetProgramInfoLog(program_, ERROR_BUFSIZE, &length, tempErrorLog);
        errors_ += "Linker errors:\n";
		errors_ += std::string(tempErrorLog, length) + "\n";
	}
}

Shader::~Shader() {
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
	glDeleteProgram(program_);
}

std::vector<char> Shader::readSource(const std::string& path) {

	// Open the file
	std::vector<char> source;
	std::ifstream in(path.c_str());
	if (in.fail()) {
		source.push_back(0);
		return source;
	}
	
	// Seek to the end of the file to get the size
	in.seekg(0, std::ios::end);
	source.reserve((unsigned)(1 + in.tellg()));
	source.resize((unsigned)in.tellg());
	in.seekg(0, std::ios::beg);
	if (source.empty()) {
		source.push_back(0);
		return source;
	}
	
	// Now read the whole buffer in one call, and don't 
	// forget to null-terminate!
	in.read(&source.front(), source.size());
	source.push_back(0);
	
	return source;
}

const std::string& Shader::path() const {
	return path_;
}

GLuint Shader::program() const {
	return program_;
}

const std::string& Shader::errors() const {
	return errors_;
}

bool Shader::loaded() const {
	return loaded_;
}
