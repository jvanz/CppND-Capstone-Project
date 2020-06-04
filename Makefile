.PHONY: format installboost

CHDIR_SHELL := $(SHELL)
define chdir
   $(eval _D=$(firstword $(1) $(@D)))
   $(info $(MAKE): cd $(_D)) $(eval SHELL = cd $(_D); $(CHDIR_SHELL))
endef

format:
	clang-format -i src/**/*
	clang-format -i src/*.{cpp,hpp}

boostdeps:
	apt-get install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev

cleanboost:
	rm -rf boost_1_66_0
	rm -f boost_1_66_0.tar.gz
    
downloadboost:
	wget https://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.gz
	tar xzvf boost_1_66_0.tar.gz

buildboost:
	cd boost_1_66_0/ &&  ./bootstrap.sh --prefix=/usr/ &&  ./b2 && ./b2 install
    
boost: boostdeps cleanboost downloadboost buildboost
