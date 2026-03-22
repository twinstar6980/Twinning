-- utility

-- ----------------

function list_contain(value, ...)
	for _, item in pairs({ ... }) do
		if item == value then
			return true
		end
	end
	return false
end

-- ----------------

function make_manifest(name, version)
	local manifest = {
		name = nil,
		version = nil,
		system = nil,
		architecture = nil,
		build = nil,
		root = nil,
	}
	manifest.name = name
	manifest.version = version
	manifest.system = 'unknown'
	if is_os('windows') then
		manifest.system = 'windows'
	end
	if is_os('linux') then
		manifest.system = 'linux'
	end
	if is_os('macosx') then
		manifest.system = 'macintosh'
	end
	if is_os('android') then
		manifest.system = 'android'
	end
	if is_os('ios') then
		manifest.system = 'iphone'
	end
	manifest.architecture = 'unknown'
	if is_arch('x86_64', 'x64') then
		manifest.architecture = 'amd64'
	end
	if is_arch('arm64', 'arm64-v8a') then
		manifest.architecture = 'arm64'
	end
	manifest.build = 'unknown'
	if is_mode('debug') then
		manifest.build = 'debug'
	end
	if is_mode('release') then
		manifest.build = 'release'
	end
	manifest.root = '$(projectdir)'
	return manifest
end

function load_manifest(target)
	return target:values('manifest')[1]
end

-- ----------------

function define_project(name, version)
	manifest = make_manifest(name, version)
	set_values('manifest', manifest, '')
	set_project(name)
	set_version(version)
	set_policy('check.auto_ignore_flags', false)
	set_policy('build.warning', true)
	set_encodings('utf-8')
	set_languages('c23', 'cxx26')
	set_symbols('hidden')
	add_rules('mode.debug', 'mode.release')
	return
end

-- ----------------

function apply_condition_definition_basic(target)
	local manifest = load_manifest(target)
	target:add(
		'defines',
		'M_version="' .. manifest.version .. '"',
		'M_system="' .. manifest.system .. '"',
		'M_system_' .. manifest.system,
		'M_architecture="' .. manifest.architecture .. '"',
		'M_architecture_' .. manifest.architecture,
		'M_build="' .. manifest.build .. '"',
		'M_build_' .. manifest.build,
		{ private = true }
	)
	return
end

function apply_compiler_option_basic(target)
	local manifest = load_manifest(target)
	target:add(
		'cxflags',
		'-fPIC', -- need for dynamic library
		{ private = true }
	)
	if list_contain(manifest.system, 'windows') then
		target:add(
			'ldflags',
			'-municode', -- enable unicode mode
			{ private = true }
		)
		target:add(
			'defines',
			'UNICODE', -- enable unicode mode
			'_UNICODE', -- enable unicode mode
			'NOMINMAX', -- suppress windows's min|max macro
			'_WINSOCKAPI_', -- suppress winsock.h
			'_UCRT_NOISY_NAN', -- enable legacy NAN macro in Windows SDK 26100+
			'_CRT_SECURE_NO_WARNINGS', -- disable secure check warning
			{ private = true }
		)
	end
	if list_contain(manifest.system, 'linux', 'macintosh', 'android', 'iphone') then
		target:add(
			'defines',
			'_GNU_SOURCE', -- enable GNU feature
			{ private = true }
		)
	end
	return
end

function apply_compiler_option_warning_disable(target)
	local manifest = load_manifest(target)
	target:add(
		'cxflags',
		'-w',
		'-Wno-c++11-narrowing',
		{ private = true }
	)
	return
end

function apply_compiler_option_warning_regular(target)
	local manifest = load_manifest(target)
	target:add(
		'cxflags',
		'-Weverything',
		'-Wno-c99-extensions',
		'-Wno-c++98-compat',
		'-Wno-c++98-compat-pedantic',
		'-Wno-c++20-compat',
		'-Wno-pre-c++14-compat',
		'-Wno-pre-c++17-compat',
		'-Wno-pre-c++26-compat',
		'-Wno-ctad-maybe-unsupported',
		'-Wno-redundant-parens',
		'-Wno-dollar-in-identifier-extension',
		'-Wno-disabled-macro-expansion',
		'-Wno-gnu-zero-variadic-macro-arguments',
		'-Wno-padded',
		'-Wno-weak-vtables',
		'-Wno-unreachable-code-return',
		'-Wno-unreachable-code-break',
		'-Wno-missing-noreturn',
		'-Wno-unused-local-typedef',
		'-Wno-unused-parameter',
		'-Wno-unused-variable',
		'-Wno-shadow',
		'-Wno-shadow-field',
		'-Wno-shadow-field-in-constructor',
		'-Wno-shadow-uncaptured-local',
		'-Wno-switch-enum',
		'-Wno-covered-switch-default',
		'-Wno-reserved-identifier',
		'-Wno-reserved-user-defined-literal',
		'-Wno-float-equal',
		'-Wno-unsafe-buffer-usage',
		'-Wno-global-constructors',
		'-Wno-exit-time-destructors',
		{ private = true }
	)
	if list_contain(manifest.system, 'windows', 'linux', 'macintosh', 'iphone') then
		target:add(
			'cxflags',
			'-Wno-nrvo',
			'-Wno-unique-object-duplication',
			'-Wno-thread-safety-analysis',
			'-Wno-thread-safety-negative',
			{ private = true }
		)
	end
	return
end
