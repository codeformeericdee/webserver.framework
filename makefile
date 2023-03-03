TESTOBJECT = Test
SERVEROBJECT = Server
CLANGHTTP = -O0
CLANGHTTPS = -O0 -lssl -lcrypto
OUTPUTDIR = Binary

cleanout:
	rm $(OUTPUTDIR)/*

testhttpbuild:
	@mkdir -p $(OUTPUTDIR) # Makes dir if non-existent.
	clang $(TESTOBJECT).c $(CLANGHTTP) -o $(OUTPUTDIR)/$(SERVEROBJECT)

httpbuild:
	@mkdir -p $(OUTPUTDIR) # Makes dir if non-existent.
	clang $(SERVEROBJECT).c $(CLANGHTTP) -o $(OUTPUTDIR)/$(SERVEROBJECT)

testhttpsbuild:
	@mkdir -p $(OUTPUTDIR) # Makes dir if non-existent.
	clang -o openssl $(TESTOBJECT).c $(CLANGHTTPS) -o $(OUTPUTDIR)/$(SERVEROBJECT)

httpsbuild:
	@mkdir -p $(OUTPUTDIR) # Makes dir if non-existent.
	clang -o openssl $(SERVEROBJECT).c $(CLANGHTTPS) -o $(OUTPUTDIR)/$(SERVEROBJECT)

runat:
	@tput setaf 1
	@echo '|'
	@tput setaf 2
	@echo '||'
	@tput setaf 3
	@echo '---'
	
	@./$(OUTPUTDIR)/$(SERVEROBJECT)

	@tput setaf 4
	@echo '----'
	@echo '||||'
	@tput setaf 5
	@echo '|||||'
	@tput setaf 6
	@echo '||||||'
	@tput setaf 7
	@echo '|||||||'
	@echo 'Application closed.'