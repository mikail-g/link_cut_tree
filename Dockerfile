FROM base AS ece8380

WORKDIR /home/mikaila/Documents/ECE8380/
RUN mkdir testing
ADD /build /home/mikaila/Documents/ECE8380/testing
WORKDIR /home/mikaila/Documents/ECE8380/testing/



ENTRYPOINT bash






