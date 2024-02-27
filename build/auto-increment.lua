--
-- Auto increment 1.0 for K-Engine!
-- February/2024
--

-- function to read k_version.hpp
function readFile(filename, mode)
	file = io.open(filename, mode)

	content = nil

	if file ~= nil then
		content = file:read("*all")
	end

	file:close()

	return content
end

-- function to write to k_version.hpp
function writeFile(filename, mode, content)
	file = io.open(filename, mode)

	if file ~= nil then
		file:write(content)
	else
		print("[ERROR] Couldn't write to the file: "..filename)
	end

	file:close()
end

-- function to get the line in version header file
function getLine(magic_word, content)
	location = content:find(magic_word)
	line = content:sub(location)
	line = line:sub(0, line:find("\n"))
	return line
end

-- function to swap line strings
function swapLines(line1, line2, content)
	content = content:gsub(line1, "_@_DUMMY_@_\n")
	content = content:gsub("_@_DUMMY_@_", line2)
	return content
end

--
--
-- MAIN CODE
--
--

-- checking environment variables

if arg[1] == nil then
	print("[ERROR] You must specify version header file name.")
	os.exit()
end

if arg[2] == nil then
	print("[ERROR] You must specify build type.")
	os.exit()
end

if arg[3] == nil then
	print("[ERROR] You must specify build platform.")
	os.exit()
end

-- opengin the version header file

version_file_content = readFile(arg[1], "r")

if version_file_content == nil then
	print("[ERROR] Couldn't open the version file: "..arg[1]..".")
	os.exit()
end

-- hash = readFile("githash", "r")

-- if hash == nil then
-- 	print("[ERROR] Couldn't open the git hash file.")
-- 	os.exit()
-- end

-- finding the increment version number to update

-- magic_word = "#define "..arg[2]
-- location = version_file_content:find(magic_word)

-- if location == nil then
-- 	print("[ERROR] Couldn't find the increment version keyword.")
-- 	os.exit()
-- end

-- s = version_file_content:sub(location + magic_word:len() + 1)

-- version_number = nil
-- new_line_location = s:find("\n")

-- if new_line_location ~= nil then
-- 	s = s:sub(0, new_line_location - 1) 
-- end

-- version_number = tonumber(s:match("%d+"))
		
-- if version_number ~= nil then
-- 	version_number = version_number + 1
-- else
-- 	print("[ERROR] Invalid increment version number.")
-- end

-- increment_version_line = version_file_content:sub(location)
-- increment_version_line = increment_version_line:sub(0, increment_version_line:find("\n"))

-- location = version_file_content:find("#define GIT_COMMIT_HASH_CODE")
-- git_hash_code_line = version_file_content:sub(location)
-- git_hash_code_line = git_hash_code_line:sub(0, git_hash_code_line:find("\n"))

-- version_file_content = version_file_content:gsub(git_hash_code_line, "_@_DUMMY_@_\n")
-- version_file_content = version_file_content:gsub("_@_DUMMY_@_", "#define GIT_COMMIT_HASH_CODE "..hash)

--
-- Updating K_ENGINE_BUILD_TYPE
-- 

build_type_line = getLine("#define K_ENGINE_BUILD_TYPE", version_file_content)

build_type = nil

if arg[2] == "Release" then
	build_type = "1"
elseif arg[2] == "Debug" then
	build_type = "2"
else
	build_type = "0"
end

version_file_content = swapLines(build_type_line, "#define K_ENGINE_BUILD_TYPE "..build_type, version_file_content)

--
-- Updating K_ENGINE_BUILD_PLATFORM
-- 

build_platform = getLine("#define K_ENGINE_BUILD_PLATFORM", version_file_content)
version_file_content = swapLines(build_platform, "#define K_ENGINE_BUILD_PLATFORM "..arg[3], version_file_content)

-- updating the version file with new increment version
-- print(version_file_content)
writeFile(arg[1], "w", version_file_content)
