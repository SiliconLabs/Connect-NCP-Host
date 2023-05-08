/***************************************************************************//**
 * @brief Connect CLI Host app for the NCP feature
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/filehistorystorage.h>

#include <cli/boostasioscheduler.h>
#include <cli/boostasioremotecli.h>

#include <connect/ncp.h>
#include "cli_commands.h"

#include "app_init.h"
#include "app_cli.h"
#include "app_common.h"

namespace cli
{
using MainScheduler = BoostAsioScheduler;
using CliTelnetServer = BoostAsioCliTelnetServer;
}

using namespace cli;
using namespace std;

int main()
{
  try
  {
    CmdHandler colorCmd;
    CmdHandler nocolorCmd;

    // setup cli

    auto rootMenu = make_unique<Menu>("cli");
    load_cli_commands(rootMenu);
    Cli cli(std::move(rootMenu), std::make_unique<FileHistoryStorage>(".cli") );
    // global exit action
    cli.ExitAction([](auto& out){
      out << "Exiting.\n";
      // Discard any previous image
      free_gbl_image();
    });
    // std exception custom handler
    cli.StdExceptionHandler(
      [](std::ostream& out, const std::string& cmd, const std::exception& e)
    {
      out << "Exception caught in cli handler: "
          << e.what()
          << " handling command: "
          << cmd
          << ".\n";
    }
      );
    SetColor();

    MainScheduler scheduler;
    CliLocalTerminalSession localSession(cli, scheduler, std::cout, 200);
    localSession.ExitAction(
      [&scheduler](auto& out)   // session exit action
    {
      out << "Closing App...\n";
      // Discard any previous image
      free_gbl_image();
      scheduler.Stop();
    }
      );

    // setup server

    CliTelnetServer server(cli, scheduler, 5000);
    // exit action for all the connections
    server.ExitAction([](auto& out) {
      out << "Terminating this session...\n";
    });

    app_init();

    scheduler.Run();

    return 0;
  }
  catch (const std::exception& e)
  {
    cerr << "Exception caugth in main: " << e.what() << '\n';
  }
  catch (...)
  {
    cerr << "Unknown exception caugth in main.\n";
  }
  return -1;
}
