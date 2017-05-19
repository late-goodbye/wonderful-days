import urllib.request, re

years = [i for i in range(2007, 2017, 1)]
months = [6, 7, 8]
url = 'https://www.gismeteo.ru/diary/4368/{}/{}'
output = open('temperatures.txt', 'w')


for month in months:
    for year in years:

        print('Processing month {} year {}'.format(month, year))
        html = ''.join(urllib.request.urlopen(url.format(year, month)).read().decode('utf-8').splitlines())
        days = re.findall('<tr align="center">.*?</tr>', html)

        pattern = re.compile(
            "<td class=first>(?P<date>\d+)</td>.*?<td class='first_in_group positive'>\+(?P<temperature>\d+)</td>")

        output.write(str(month) + ' ' + str(year))
        for day in days:
            m = pattern.search(day)
            if not (month == 6 and int(m.group('date')) < 15 or month == 8 and int(m.group('date')) > 15):
                output.write(' ' + m.group('date') + ' ' + m.group('temperature'))
        output.write('\n')