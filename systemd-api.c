#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>
#include <systemd/sd-journal.h>

int get_journactl(void)
{
     sd_journal *jrnl;
     const char *d;
     size_t l;
                  
     int r = sd_journal_open(&jrnl, SD_JOURNAL_LOCAL_ONLY);
     if (r < 0) {
          fprintf(stderr, "Failed to open journal: %s\n", strerror(-r));
          goto out;
     }

     SD_JOURNAL_FOREACH(jrnl) {

          r = sd_journal_get_data(jrnl, "MESSAGE", (const void **)&d, &l);
          if (r < 0) {
               fprintf(stderr, "Failed to read message field: %s\n", strerror(-r));
               continue;
          }

          fprintf(stdout, "%s%s", d, "\n");
     }

out:
    sd_journal_close(jrnl);
    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

int get_nfailed_units(void)
{
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus *bus = NULL;
    sd_bus_message *reply = NULL;
    int NFailedUnits;

    int r = sd_bus_default_system(&bus);
    if (r < 0) {
        fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
        goto out;
    }

    r = sd_bus_get_property(bus,"org.freedesktop.systemd1",
                                "/org/freedesktop/systemd1",
                                "org.freedesktop.systemd1.Manager",
                                "NFailedUnits",
                                &error,
                                &reply, 
                                "u");

    if (r < 0) {
        fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
        goto out;
    }

    r = sd_bus_message_read(reply, "u", &NFailedUnits);
    if (r < 0) {
        fprintf(stderr, "Failed to parse reply: %s\n", strerror(-r));
        goto out;
    }

    fprintf(stdout, "Number of failed units: %d\n", NFailedUnits);

out:
    sd_bus_error_free(&error);
    sd_bus_unref(bus);
    sd_bus_message_unref(reply);

    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;

}

int main(int argc, char **argv) 
{

    get_journactl();

    get_nfailed_units();

    return 0;
}
