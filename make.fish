make -C lib/linux/ clean || exit 1
# bear --append -- make -C lib/linux/ || exit 1
make -C lib/linux/ || exit 1
for d in (ls apps);
        make -C apps/$d/linux clean || exit 1
end
for d in (ls apps);
        # bear --append -- make -C apps/$d/linux || exit 1
        make -C apps/$d/linux || exit 1
end
