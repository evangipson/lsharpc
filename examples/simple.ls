/* grab the io library for it's logging functions */
grab io

/* A normal logging function */
io.log('A normal call to io.log from L#.')

/* Full suite of named log functions */
io.debug('A normal call to io.debug from L#.')
io.information('A normal call to io.information from L#.')
io.warning('A normal call to io.warning from L#.')
io.error('A normal call to io.error from L#.')

/* declaring and assiging the number type */
number temperature = 98.4

/* logging the number that was assigned */
io.information('Temperature set: `temperature`')