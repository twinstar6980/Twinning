-- helper

function create_m()
	return {
		root = nil,
		system = {
			name = nil,
			is = function(self, ...)
				for key, value in pairs({ ... }) do
					if value == self.name then
						return true
					end
				end
				return false
			end,
		},
		architecture = {
			name = nil,
			is = function(self, ...)
				for key, value in pairs({ ... }) do
					if value == self.name then
						return true
					end
				end
				return false
			end,
		},
		build = {
			name = nil,
			is = function(self, ...)
				for key, value in pairs({ ... }) do
					if value == self.name then
						return true
					end
				end
				return false
			end,
		},
	}
end

function make_m()
	local m = create_m()
	m.root = '$(projectdir)'
	m.system.name = 'unknown'
	if is_os('windows') then
		m.system.name = 'windows'
	end
	if is_os('linux') then
		m.system.name = 'linux'
	end
	if is_os('macosx') then
		m.system.name = 'macintosh'
	end
	if is_os('android') then
		m.system.name = 'android'
	end
	if is_os('ios') then
		m.system.name = 'iphone'
	end
	m.architecture.name = 'unknown'
	if is_arch('x86', 'i386') then
		m.architecture.name = 'x86_32'
	end
	if is_arch('x86_64', 'x64') then
		m.architecture.name = 'x86_64'
	end
	if is_arch('arm', 'armv7', 'armeabi', 'armeabi-v7a') then
		m.architecture.name = 'arm_32'
	end
	if is_arch('arm64', 'arm64-v8a') then
		m.architecture.name = 'arm_64'
	end
	m.build.name = 'unknown'
	if is_mode('debug') then
		m.build.name = 'debug'
	end
	if is_mode('release') then
		m.build.name = 'release'
	end
	return m
end

function load_m(target)
	local m_list = target:values('m')
	local m = create_m()
	m.root = m_list[1]
	m.system.name = m_list[2]
	m.architecture.name = m_list[3]
	m.build.name = m_list[4]
	return m
end

function apply_common_setting()
	m = make_m()
	set_values('m', m.root, m.system.name, m.architecture.name, m.build.name)
	set_policy('check.auto_ignore_flags', false)
	set_policy('build.warning', true)
	set_encodings('utf-8')
	set_languages('c23', 'cxx26')
	set_symbols('hidden')
end

function apply_condition_definition_basic(target)
	local m = load_m(target)
	target:add(
		'defines',
		'M_system_' .. m.system.name,
		'M_system="' .. m.system.name .. '"',
		'M_architecture_' .. m.architecture.name,
		'M_architecture="' .. m.architecture.name .. '"',
		'M_build_' .. m.build.name,
		'M_build="' .. m.build.name .. '"',
		{ private = true }
	)
end

function apply_compiler_option_basic(target)
	local m = load_m(target)
	target:add(
		'cxflags',
		'-fPIC', -- need for dynamic library
		{ private = true }
	)
	if m.system:is('windows') then
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
	if m.system:is('linux', 'macintosh', 'android', 'iphone') then
		target:add(
			'defines',
			'_GNU_SOURCE', -- enable GNU feature
			{ private = true }
		)
	end
end

function apply_compiler_option_warning_disable(target)
	local m = load_m(target)
	target:add(
		'cxflags',
		'-w',
		'-Wno-c++11-narrowing',
		{ private = true }
	)
end

function apply_compiler_option_warning_regular(target)
	local m = load_m(target)
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
		'-Wno-padded',
		'-Wno-redundant-parens',
		'-Wno-unreachable-code-return',
		'-Wno-unreachable-code-break',
		'-Wno-missing-noreturn',
		'-Wno-missing-field-initializers',
		'-Wno-unused-parameter',
		'-Wno-unused-variable',
		'-Wno-unused-local-typedef',
		'-Wno-ctad-maybe-unsupported',
		'-Wno-global-constructors',
		'-Wno-exit-time-destructors',
		'-Wno-weak-vtables',
		'-Wno-shadow',
		'-Wno-shadow-field',
		'-Wno-shadow-field-in-constructor',
		'-Wno-shadow-uncaptured-local',
		'-Wno-switch-enum',
		'-Wno-switch-default',
		'-Wno-covered-switch-default',
		'-Wno-self-assign',
		'-Wno-float-equal',
		'-Wno-unsafe-buffer-usage',
		'-Wno-gnu-line-marker',
		'-Wno-gnu-zero-variadic-macro-arguments',
		'-Wno-disabled-macro-expansion',
		'-Wno-dollar-in-identifier-extension',
		'-Wno-language-extension-token',
		'-Wno-deprecated-declarations',
		'-Wno-reserved-user-defined-literal',
		{ private = true }
	)
end
