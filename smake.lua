local import = import('smake/libraryInstaller')
local gppCompiler = import('smake/gpp')
import('smake/dependencyInstaller', true)
import('smake/dependencyIncluder', true)

function smake.install()
    InstallDependencies('openssl', 'curl')
end

function smake.build()
    gppCompiler():makeGlobal()
    standard('c++2a')
    flags('-c')
    include('include')
    IncludeDependency('curl')
    input('src/*.cpp')
    compile()

    run('ar rvs libhttp.a http_request.o http_response.o http.o')
    run('rm *.o')
end